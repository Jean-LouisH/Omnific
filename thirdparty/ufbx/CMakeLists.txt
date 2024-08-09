# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

add_library(ufbx STATIC 
	  "ufbx.c"
	  "ufbx.h"
)