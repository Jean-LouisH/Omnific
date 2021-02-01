# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project("yaml_cpp")

# Include sub-projects.
add_subdirectory("yaml-cpp-0.6.3")
