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
     "../SDL2-2.28.5/include"
)

add_library(libretti SHARED
    ${libretti_sources}
    ${libretti_headers}
)

 add_definitions(/bigobj)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../../thirdparty/SDL2-2.28.5/lib/x64)

target_link_libraries (libretti
		${SDL2}
)

add_definitions(-D_LIBRETTI_EXPORTS)

add_custom_command(TARGET libretti POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libretti> ${PROJECT_BINARY_DIR}/src/main/libretti.dll)