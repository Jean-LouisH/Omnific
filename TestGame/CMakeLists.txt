# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (TEST_GAME_SOURCES
		main.cpp
		SceneDefiner.cpp
		SceneDefiner.hpp
		QuitOnEscapeKey.cpp
)

# Add source to this project's executable.
add_executable (TestGame ${TEST_GAME_SOURCES})

# TODO: Add tests and install targets if needed.
include_directories("../Lilliputian")
include_directories("../ThirdParty/SDL2/SDL2-2.0.10/include")
include_directories("../ThirdParty/SDL2_Image/SDL2_image-2.0.5/include")
include_directories("../ThirdParty/SDL2_Mixer/SDL2_mixer-2.0.4/include")

add_dependencies (TestGame 
		Lilliputian
)

target_link_libraries (TestGame
		Lilliputian,
		SDL2,
		SDL2_image,
		SDL2_mixer
)
