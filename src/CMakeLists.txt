# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("esi")
project ("application")
project ("steam_application")
project ("dedicated_gpu_application")
project ("dedicated_gpu_steam_application")
project ("esi_native")

# Include sub-projects.
add_subdirectory ("engine")
add_subdirectory ("main")
add_subdirectory("editor")