# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (OMNIFIC_VERSION 0.1.0)

project ("omnific_engine" VERSION ${OMNIFIC_VERSION} LANGUAGES CXX)
project ("omnific_studio" VERSION ${OMNIFIC_VERSION} LANGUAGES CXX)
project ("omnific_launcher" LANGUAGES CXX)

enable_testing()

# Include sub-projects.
add_subdirectory("src/omnific_engine")
add_subdirectory("src/omnific_studio")
add_subdirectory("src/omnific_demos")
add_subdirectory("src/omnific_launcher")
add_subdirectory("thirdparty")