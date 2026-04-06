#!/usr/bin/env python3
"""Capture Linux output-audio monitor and write low/mid/high/beat to visualizer_input.txt."""

import argparse
import os
import subprocess
import sys
import time
from typing import List, Tuple

try:
    import numpy as np
except ImportError as exc:  # pragma: no cover
    raise SystemExit(
        "numpy is required. Install with: pip install numpy\n"
        "or run: ./scripts/run_system_audio_visualizer.sh"
    ) from exc


def run_cmd(args: List[str]) -> str:
    return subprocess.check_output(args, text=True).strip()


def list_sources() -> List[str]:
    output = run_cmd(["pactl", "list", "short", "sources"])
    sources: List[str] = []
    for line in output.splitlines():
        parts = line.split("\t")
        if len(parts) >= 2:
            sources.append(parts[1])
    return sources


def pick_monitor_source() -> str:
    sources = list_sources()
    if not sources:
        raise RuntimeError("No PulseAudio/PipeWire sources found")

    default_sink = run_cmd(["pactl", "get-default-sink"])
    default_monitor = f"{default_sink}.monitor"

    if default_monitor in sources:
        return default_monitor

    monitor_sources = [s for s in sources if s.endswith(".monitor")]
    if monitor_sources:
        return monitor_sources[0]

    raise RuntimeError("Could not find monitor source. Ensure output sink monitoring is enabled.")


def band_energy(freqs: np.ndarray, spectrum: np.ndarray, fmin: float, fmax: float) -> float:
    mask = (freqs >= fmin) & (freqs < fmax)
    if not np.any(mask):
        return 0.0
    return float(np.mean(spectrum[mask]))


def clamp01(value: float) -> float:
    if value < 0.0:
        return 0.0
    if value > 1.0:
        return 1.0
    return value


def write_values(path: str, low: float, mid: float, high: float, beat: float) -> None:
    tmp_path = f"{path}.tmp"
    with open(tmp_path, "w", encoding="utf-8") as fp:
        fp.write(f"{low:.4f} {mid:.4f} {high:.4f} {beat:.1f}\n")
    os.replace(tmp_path, path)


def main() -> int:
    parser = argparse.ArgumentParser(description="Output-audio to visualizer bands")
    parser.add_argument("--output", default="data/visualizer_input.txt")
    parser.add_argument("--rate", type=int, default=44100)
    parser.add_argument("--chunk", type=int, default=2048)
    parser.add_argument("--source", default="", help="Override source (otherwise auto-select monitor)")
    args = parser.parse_args()

    source = args.source or pick_monitor_source()
    print(f"Using monitor source: {source}")

    cmd = [
        "parec",
        "-d",
        source,
        "--format=s16le",
        "--rate",
        str(args.rate),
        "--channels",
        "1",
        "--raw",
    ]

    proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    if proc.stdout is None:
        raise RuntimeError("Failed to open parec stdout")

    bytes_per_sample = 2
    bytes_per_chunk = args.chunk * bytes_per_sample

    freqs = np.fft.rfftfreq(args.chunk, d=1.0 / float(args.rate))
    window = np.hanning(args.chunk)

    out_low = 0.0
    out_mid = 0.0
    out_high = 0.0

    max_low = 1e-6
    max_mid = 1e-6
    max_high = 1e-6

    prev_low = 0.0
    flux_avg = 0.0
    beat_cooldown = 0.0

    chunk_seconds = float(args.chunk) / float(args.rate)
    smooth = 0.20
    max_decay = 0.996

    try:
        while True:
            raw = proc.stdout.read(bytes_per_chunk)
            if len(raw) < bytes_per_chunk:
                time.sleep(0.005)
                continue

            samples = np.frombuffer(raw, dtype=np.int16).astype(np.float32) / 32768.0
            spectrum = np.abs(np.fft.rfft(samples * window))

            low_raw = band_energy(freqs, spectrum, 20.0, 250.0)
            mid_raw = band_energy(freqs, spectrum, 250.0, 2000.0)
            high_raw = band_energy(freqs, spectrum, 2000.0, 12000.0)

            max_low = max(max_low * max_decay, low_raw)
            max_mid = max(max_mid * max_decay, mid_raw)
            max_high = max(max_high * max_decay, high_raw)

            norm_low = clamp01(low_raw / (max_low + 1e-8))
            norm_mid = clamp01(mid_raw / (max_mid + 1e-8))
            norm_high = clamp01(high_raw / (max_high + 1e-8))

            out_low += (norm_low - out_low) * smooth
            out_mid += (norm_mid - out_mid) * smooth
            out_high += (norm_high - out_high) * smooth

            if beat_cooldown > 0.0:
                beat_cooldown -= chunk_seconds

            low_diff = max(0.0, out_low - prev_low)
            prev_low = out_low
            flux_avg = flux_avg * 0.92 + low_diff * 0.08

            beat = 0.0
            if low_diff > flux_avg * 1.9 and out_low > 0.5 and beat_cooldown <= 0.0:
                beat = 1.0
                beat_cooldown = 0.18

            write_values(args.output, out_low, out_mid, out_high, beat)
    except KeyboardInterrupt:
        print("\nStopped audio bridge")
    finally:
        proc.terminate()
        proc.wait(timeout=1)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
