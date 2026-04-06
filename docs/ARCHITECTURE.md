# Refactor Architecture Notes

## Why this refactor
The original project had gameplay, input, animation, and rendering logic mixed in one file with many mutable globals. That made it hard to reason about state updates and hard to isolate bugs.

This refactor introduces a separation of concerns so each part of the program has one clear role.

## New modules

### include/world_state.h
Holds runtime state in one place.

Examples:
- Camera position, angles, and mode flags
- Train positions and speed
- Barrier angle and stop-light state

Design value:
- Makes ownership of mutable data explicit
- Avoids scattered state edits in unrelated code paths

### include/input_controller.h / src/input_controller.cpp
Owns input-to-state transformations.

Responsibilities:
- WASD motion updates for camera
- Pan mode toggle (F3 behavior)
- Mouse look deltas

Design value:
- Input policy is centralized
- Easier to tune controls without touching render code

### include/animation_system.h / src/animation_system.cpp
Owns time-step logic.

Responsibilities:
- Pan animation increments
- Linear train movement and direction switching
- Barrier animation and stop-light synchronization

Design value:
- Animation math is isolated from drawing
- Timer callbacks become thin and predictable

### include/renderer.h / src/renderer.cpp
Owns camera matrix setup.

Responsibilities:
- Translate state into view transform through gluLookAt

Design value:
- Rendering decisions can evolve independently from gameplay and input

### include/visualizer_system.h / src/visualizer_system.cpp
Owns audio-band ingestion and beat/pulse derivation.

Responsibilities:
- Read low/mid/high/beat values from [data/visualizer_input.txt](data/visualizer_input.txt)
- Provide smoothed band values to rendering code
- Detect beat pulses (explicit beat hint and low-band peak fallback)
- Provide procedural fallback when no external data is present

Design value:
- Keeps audio-reactive logic out of rendering primitives
- Makes it easy to swap file-based input for microphone/system-loopback later
- Stabilizes visuals with smoothing and pulse decay

### scripts/system_audio_to_visualizer.py (bridge)
Linux helper process that captures system output audio from sink monitor (`pactl` + `parec`),
extracts low/mid/high + beat in near real-time, and writes values into [data/visualizer_input.txt](data/visualizer_input.txt).

Design value:
- External process boundary keeps OpenGL render loop simple.
- Audio capture and FFT failures do not crash core rendering logic.
- Works with any application that plays to desktop output (browser, Spotify, media players).

## Teaching summary

### 1) Keep one source of truth for mutable state
Moving dynamic values into WorldState avoids hidden coupling and makes debugging less random.

### 2) Keep update systems separate
Input and animation are conceptually different:
- Input is event-driven (keys, mouse)
- Animation is time-driven (timer ticks)

Keeping them in separate classes avoids accidental side effects.

### 3) Keep callbacks thin
GLUT callbacks now delegate to modules. Thin callbacks reduce bug surface area and make testing easier.

## Build impact
Build scripts now compile multiple translation units:
- src/csk29.cpp
- src/input_controller.cpp
- src/animation_system.cpp
- src/renderer.cpp

No runtime behavior change is intended; this is a structural refactor focused on maintainability.
