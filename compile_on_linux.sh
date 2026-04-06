# Linux Ubuntu required before installed libraries
# sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
g++ csk29.cpp input_controller.cpp animation_system.cpp renderer.cpp barrier_renderer.cpp visualizer_system.cpp -o gl -std=c++0x -W -Wall -Wextra -pedantic -lGL -lGLU -lglut
