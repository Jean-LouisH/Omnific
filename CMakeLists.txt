# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Include sub-projects.
add_subdirectory("src/engine")
add_subdirectory("src/studio")
add_subdirectory("src/demo")
add_subdirectory("src/launcher")
add_subdirectory("thirdparty")