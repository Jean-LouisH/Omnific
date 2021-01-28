# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE yaml_cpp_sources "src/[a-zA-Z]*.cpp")
file(GLOB_RECURSE yaml_cpp_public_headers "include/yaml-cpp/[a-zA-Z]*.h")
file(GLOB_RECURSE yaml_cpp_private_headers "src/[a-zA-Z]*.h")

include_directories(
	 "include/yaml-cpp/"
	 "include"
)

add_library(yaml_cpp STATIC 
	  ${yaml_cpp_sources}
	  ${yaml_cpp_public_headers}
	  ${yaml_cpp_private_headers}
)