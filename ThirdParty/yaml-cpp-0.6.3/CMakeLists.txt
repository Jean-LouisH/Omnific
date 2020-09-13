# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB sources "src/[a-zA-Z]*.cpp")
file(GLOB_RECURSE public_headers "include/yaml-cpp/[a-zA-Z]*.h")
file(GLOB private_headers "src/[a-zA-Z]*.h")

include_directories(
	 "include/yaml-cpp/"
	 "include"
)

add_library(YAML_CPP STATIC 
	  ${sources}
	  ${public_headers}
	  ${private_headers}
)