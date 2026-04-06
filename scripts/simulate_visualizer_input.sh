#!/usr/bin/env zsh
# Generates synthetic low/mid/high/beat values for visualizer_input.txt.
set -euo pipefail

cd "$(dirname "$0")/.."

target_file="data/visualizer_input.txt"
phase=0

while true; do
  low=$(awk -v p="$phase" 'BEGIN { v = 0.5 + 0.45 * sin(p * 0.12); if (v < 0) v=0; if (v > 1) v=1; printf "%.3f", v }')
  mid=$(awk -v p="$phase" 'BEGIN { v = 0.5 + 0.45 * sin(p * 0.17 + 1.1); if (v < 0) v=0; if (v > 1) v=1; printf "%.3f", v }')
  high=$(awk -v p="$phase" 'BEGIN { v = 0.5 + 0.45 * sin(p * 0.23 + 2.2); if (v < 0) v=0; if (v > 1) v=1; printf "%.3f", v }')
  beat=$(awk -v l="$low" 'BEGIN { if (l > 0.82) printf "1.0"; else printf "0.0" }')

  printf "%s %s %s %s\n" "$low" "$mid" "$high" "$beat" > "$target_file"
  phase=$((phase + 1))
  sleep 0.03
done
