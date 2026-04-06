#!/usr/bin/env zsh
# Captures system output audio (sink monitor) and writes low/mid/high/beat to visualizer_input.txt.
set -euo pipefail

if [[ ! -d .venv_audio ]]; then
  python3 -m venv .venv_audio
fi

source .venv_audio/bin/activate
python3 -m pip install --upgrade pip >/dev/null
python3 -m pip install numpy >/dev/null

python3 ./system_audio_to_visualizer.py "$@"
