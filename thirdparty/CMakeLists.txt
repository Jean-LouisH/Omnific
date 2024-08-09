# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project("glad" LANGUAGES C)
project("libretti" LANGUAGES C)
project("ufbx" LANGUAGES C)
project("yaml_cpp" LANGUAGES CXX)

# Include sub-projects.
add_subdirectory("glad")
add_subdirectory("libretti")
add_subdirectory("ufbx")
add_subdirectory("yaml-cpp")
