# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE libretti_sources "src/[a-zA-Z]*.c")
file(GLOB_RECURSE libretti_headers "include/[a-zA-Z]*.h")

include_directories(
	 "../SDL2-2.0.12/include"
	 "include/"
)

add_library(Libretti STATIC 
	  ${libretti_sources}
	  ${libretti_headers}
)