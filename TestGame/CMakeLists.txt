# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (TEST_GAME_SOURCES
		main.cpp
		SceneDefiner.cpp
		SceneDefiner.hpp
		QuitOnEscapeKey.cpp
		QuitOnEscapeKey.hpp
)

# Add source to this project's executable.
add_executable (TestGame ${TEST_GAME_SOURCES} "ReturnToMenu.hpp" "ReturnToMenu.cpp" "PongClone/GenerateBall.cpp" "PongClone/PongClone.hpp" "PongClone/ManageScore.cpp" "PongClone/ShakeCamera.cpp" "PongClone/ControlPlayer.cpp" "PongClone/ControlAI.cpp")

include_directories(
		"../Lilliputian"
		"../ThirdParty/SDL2-2.0.12/include"
		"../ThirdParty/SDL2_image-2.0.5/include"
		"../ThirdParty/SDL2_mixer-2.0.4/include")

add_dependencies (TestGame 
		Lilliputian
)

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../ThirdParty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../ThirdParty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../ThirdParty/SDL2_mixer-2.0.4/lib/x64)

target_link_libraries (TestGame
		Lilliputian
		${SDL2}
)
