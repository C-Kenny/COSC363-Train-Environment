# Linux Ubuntu required before installed libraries
# sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
g++ src/csk29.cpp src/input_controller.cpp src/animation_system.cpp src/renderer.cpp src/barrier_renderer.cpp src/visualizer_system.cpp -Iinclude -o gl -std=c++0x -W -Wall -Wextra -pedantic -lGL -lGLU -lglut
