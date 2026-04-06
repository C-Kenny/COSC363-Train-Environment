# COSC363-Train-Environment

## What is it?
Graphics project made with C++/OpenGL/glut/glew

## Can I see a demo without spending 4 hours getting glut setup?
[.webm demo hosted by me](http://carlkenny.com/media/train_c++_demo.webm)



## TODO:
- Extra features
    * Add object collisions for barrier arms
    * Change dance floor to react to user's music

- Refactor
    * Break into logic and view sections
    * Split into multiple files with headers

- Fix bugs:
    * Investigate existing mouse bug where looking up too high creates mouse issues
    * Barrier arm lights don't glow. Only reflect from center light
    * Poor performance on OSX. Runs at a high FPS on Linux however

## Refactor update (April 2026)

This project has been partially modernized into an OOP/module architecture to make future changes safer and faster.

### New files
- include/world_state.h
- include/input_controller.h / src/input_controller.cpp
- include/animation_system.h / src/animation_system.cpp
- include/renderer.h / src/renderer.cpp
- docs/ARCHITECTURE.md

### What changed and why
- State was centralized into WorldState so mutable runtime data is no longer spread across unrelated logic.
- Input behavior moved into InputController so key/mouse policy is isolated from rendering.
- Time-based movement moved into AnimationSystem so timer callbacks only trigger deterministic state updates.
- Camera transform setup moved into Renderer to decouple presentation from gameplay logic.

### Build scripts
Both compile scripts now compile multiple source files instead of one monolithic cpp.

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for a teaching-oriented walkthrough.

## Music Visualizer Mode (new)

The scene now reacts to four values read from [data/visualizer_input.txt](data/visualizer_input.txt):

1. `low` in `[0, 1]`
2. `mid` in `[0, 1]`
3. `high` in `[0, 1]`
4. optional `beat` in `[0, 1]`

Format:

```txt
0.63 0.41 0.77 1.0
```

Reactive mapping:
- Speaker towers scale with low frequencies and pulse on beats.
- Speaker face rings expand and recolor with mid/high frequencies.
- Dance floor colors now track low/mid/high bands.
- A pulse sphere flashes around the dance floor on stronger beat energy.

If no external file updates are provided, the app uses a procedural fallback signal so the visualizer remains animated.

### Quick demo signal

Run the app in one terminal, and in another terminal run:

```bash
chmod +x ./scripts/simulate_visualizer_input.sh
./scripts/simulate_visualizer_input.sh
```

This continuously updates [data/visualizer_input.txt](data/visualizer_input.txt) with synthetic band values.

### Real system-output audio (Spotify / YouTube / Firefox)

This project now includes a Linux output-audio bridge that reads the default sink monitor
(not microphone input) and converts it to low/mid/high/beat values.

Files:
- [scripts/system_audio_to_visualizer.py](scripts/system_audio_to_visualizer.py)
- [scripts/run_system_audio_visualizer.sh](scripts/run_system_audio_visualizer.sh)

Run flow:

1. In terminal A:

```bash
./compile_on_linux.sh
./gl 2
```

2. In terminal B:

```bash
chmod +x ./scripts/run_system_audio_visualizer.sh
./scripts/run_system_audio_visualizer.sh
```

That bridge auto-selects your default sink monitor using `pactl`, so any desktop output
(Spotify desktop/web, YouTube in Firefox, etc.) drives the scene.

Notes:
- Uses PulseAudio/PipeWire tools: `pactl`, `parec`.
- The wrapper creates `.venv_audio` and installs `numpy` automatically.
- Stop bridge with `Ctrl+C`.
