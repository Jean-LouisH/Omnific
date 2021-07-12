# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE glad_cpp_sources "src/[a-zA-Z]*.c")
file(GLOB_RECURSE glad_cpp_public_headers "include/[a-zA-Z]*.h")
file(GLOB_RECURSE glad_cpp_private_headers "src/[a-zA-Z]*.h")

include_directories(
	 "include"
)

add_library(glad STATIC 
	  ${glad_cpp_sources}
	  ${glad_cpp_public_headers}
	  ${glad_cpp_private_headers}
)