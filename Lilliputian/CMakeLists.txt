# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(LILLIPUTIAN_SOURCES
		Lilliputian.hpp
		Core/Core.hpp
		Core/Core.cpp
		Engines/AIEngine/AIEngine.hpp
		Engines/AIEngine/AIEngine.cpp
		Engines/AnimationEngine/AnimationEngine.hpp
		Engines/AnimationEngine/AnimationEngine.cpp
		Engines/AudioEngine/AudioEngine.cpp
		Engines/AudioEngine/AudioEngine.hpp
		Engines/AudioEngine/AudioSDL/AudioSDL.cpp
		Engines/AudioEngine/AudioSDL/AudioSDL.hpp
		Engines/HapticEngine/HapticEngine.hpp
		Engines/HapticEngine/HapticEngine.cpp
		Engines/PhysicsEngine/PhysicsEngine.cpp
		Engines/PhysicsEngine/PhysicsEngine.hpp
		Engines/RenderingEngine/RenderingEngine.hpp
		Engines/RenderingEngine/RenderingEngine.cpp
		Engines/RenderingEngine/RenderingSDL/Camera2D.hpp
		Engines/RenderingEngine/RenderingSDL/Rect.hpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.cpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.hpp
		Engines/RenderingEngine/RenderingSDL/SDLRenderable.hpp
		Engines/RenderingEngine/RenderingSDL/Sprite2D.hpp
		Engines/RenderingEngine/RenderingSDL/Texture.hpp
		Engines/RenderingEngine/RenderingSDL/Transform2D.hpp
		Engines/RenderingEngine/RenderingSDL/Vector2.hpp
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
		Utilities/GenericCollections/Map.hpp
		Utilities/GenericCollections/Queue.hpp
		Utilities/GenericCollections/Stack.hpp
		Utilities/GenericCollections/String.hpp
		Utilities/GenericCollections/Vector.hpp
)

# Add source to this project's executable.
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES})

include_directories(
		"../ThirdParty/SDL2/SDL2-2.0.10/include"
		"../ThirdParty/SDL2_Image/SDL2_image-2.0.5/include"
		"../ThirdParty/SDL2_Mixer/SDL2_mixer-2.0.4/include")

# TODO: Add tests and install targets if needed.