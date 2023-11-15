# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project("yaml_cpp")
project("glad")
project("libretti")

# Include sub-projects.
add_subdirectory("yaml-cpp-0.6.3")
add_subdirectory("glad")
add_subdirectory("libretti")
