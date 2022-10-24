# CMakeList.txt : CMake project for game-engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.21)

add_library (PhysicsEngine core.cpp pcontact.cpp pcontactresolver.cpp groundcontact.cpp)
target_include_directories (PhysicsEngine INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
