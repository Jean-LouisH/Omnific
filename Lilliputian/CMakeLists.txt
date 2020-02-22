# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(LILLIPUTIAN_SOURCES
		Lilliputian.hpp
		Core/Core.hpp
		Core/Core.cpp
		Game/ActionInputs.cpp
		Game/ActionInputs.hpp
		Game/Entity2D.cpp
		Game/Entity2D.hpp
		Game/Game.hpp
		Game/Game.cpp
		Game/Scene.cpp
		Game/Scene.hpp
		Game/Subscene2D.hpp
		Game/Subscene2D.cpp
		Game/Script.cpp
		Game/Script.hpp
		UserAPIs/EditorAPI.cpp
		UserAPIs/EditorAPI.hpp
		UserAPIs/GameAPI.cpp
		UserAPIs/GameAPI.hpp
		UserAPIs/InputAPI.cpp
		UserAPIs/InputAPI.hpp
		UserAPIs/ScriptingAPI.hpp
		UserAPIs/ScriptingAPI.cpp
		Utilities/Definitions/Constants.hpp
		Utilities/Definitions/Aliases.hpp
		Utilities/Mathematics/AABB2D.hpp
		Utilities/Mathematics/Colour.cpp
		Utilities/Mathematics/Colour.hpp
		Utilities/Mathematics/Mathematics.hpp
		Utilities/Mathematics/Rectangle.hpp
		Utilities/Mathematics/Transform2D.hpp
		Utilities/Mathematics/Vector2.hpp
)

# Add source to this project's executable.
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES})

include_directories("../ThirdParty/SDL2/SDL2-2.0.10/include")
include_directories("../ThirdParty/SDL2_Image/SDL2_image-2.0.5/include")
include_directories("../ThirdParty/SDL2_Mixer/SDL2_mixer-2.0.4/include")

target_link_libraries (Lilliputian
		SDL2,
		SDL2_image,
		SDL2_mixer
)

# TODO: Add tests and install targets if needed.
