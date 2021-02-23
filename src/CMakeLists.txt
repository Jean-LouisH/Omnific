# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("lilliputian_engine")
project ("lilliputian_main")

# Include sub-projects.
add_subdirectory ("lilliputian_engine")
add_subdirectory ("lilliputian_main")