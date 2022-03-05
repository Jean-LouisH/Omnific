# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("omnia_engine")
project ("application")
project ("steam_application")
project ("dedicated_gpu_application")
project ("dedicated_gpu_steam_application")
project ("omnia_cpp_native_assembly")

# Include sub-projects.
add_subdirectory("src")
add_subdirectory("thirdparty")
add_subdirectory("data/editor/assets/scripts/cpp")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT application)