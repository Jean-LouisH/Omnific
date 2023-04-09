# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE libretti_sources "./[a-zA-Z]*.c")
file(GLOB_RECURSE libretti_headers "./include/[a-zA-Z]*.h")


include_directories(
	 "."
     "../SDL2-2.26.2/include"
)

add_library(libretti STATIC
    ${libretti_sources}
    ${libretti_headers}
)
