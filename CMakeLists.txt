# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("TestGame")
project ("Lilliputian")

# Include sub-projects.
add_subdirectory("TestGame")
add_subdirectory ("Lilliputian")