# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Lilliputian")
project ("LilliputianTests")

# Include sub-projects.
add_subdirectory ("Lilliputian")
add_subdirectory("LilliputianTests")
add_subdirectory("ThirdParty")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LilliputianTests)