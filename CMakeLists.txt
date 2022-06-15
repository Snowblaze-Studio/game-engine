# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.21)

project (Engine VERSION 0.1)

# Include sub-projects.
add_subdirectory ("game-engine")

target_include_directories(Engine PRIVATE "external/sdl/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x64/SDL2main.lib")
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x64/SDL2.dll" DESTINATION "${CMAKE_BINARY_DIR}/game-engine")
else ()
  set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x86/SDL2main.lib")
  file(COPY "${CMAKE_CURRENT_LIST_DIR}/external/sdl/lib/x86/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif ()

target_link_libraries(Engine ${SDL2_LIBRARIES})