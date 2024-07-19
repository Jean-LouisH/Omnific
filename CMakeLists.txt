# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("omnific_engine")
project ("omnific_studio")
project ("omnific_demos")
project ("omnific_launcher")

enable_testing()

# Include sub-projects.
add_subdirectory("src/omnific_engine")
add_subdirectory("src/omnific_studio")
add_subdirectory("src/omnific_demos")
add_subdirectory("src/omnific_launcher")
add_subdirectory("thirdparty")
add_subdirectory("data")