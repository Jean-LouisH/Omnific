# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (TEST_GAME_SOURCES
		main.cpp
)

# Add source to this project's executable.
add_executable (TestGame ${TEST_GAME_SOURCES})

# TODO: Add tests and install targets if needed.
include_directories("../Lilliputian")

add_dependencies (TestGame 
		Lilliputian
)

target_link_libraries (TestGame
		Lilliputian
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TestGame)
