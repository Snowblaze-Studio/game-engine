# CMakeList.txt : CMake project for game-engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

# Add source to this project's executable.
add_executable (Engine "game-engine.cpp" "game-engine.hpp" "Game.hpp" "Game.cpp")

# TODO: Add tests and install targets if needed.
