# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(LILLIPUTIAN_SOURCES
		Lilliputian.hpp
		Core/Core.hpp
		Core/Core.cpp
		API/API.hpp
		API/API.cpp
)

# Add source to this project's executable.
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES})

# TODO: Add tests and install targets if needed.
