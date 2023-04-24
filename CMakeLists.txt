# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

project (Engine VERSION 0.1)

# Include external dependencies
add_subdirectory (external)
# Include sub-projects.
add_subdirectory (game-engine)

target_link_libraries(Engine PRIVATE external)