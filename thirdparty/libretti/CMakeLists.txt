# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

file(GLOB_RECURSE libretti_sources "./[a-zA-Z]*.c")
file(GLOB_RECURSE libretti_headers "./include/[a-zA-Z]*.h")


include_directories(
	 "."
     "./include"
)

add_library(libretti STATIC
    ${libretti_sources}
    ${libretti_headers}
)

if (WIN32)
add_definitions(/bigobj)
endif ()

find_package(SDL2 REQUIRED)

target_link_libraries (libretti
    SDL2::SDL2
)

#add_definitions(-D_LIBRETTI_EXPORTS)
add_definitions(-D_LIBRETTI_EXPORTS_BUILD)