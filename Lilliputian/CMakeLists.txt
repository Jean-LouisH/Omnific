# CMakeList.txt : CMake project for Lilliputian, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(LILLIPUTIAN_SOURCES
		Engine.hpp
		Engine.cpp
		EngineState.cpp
		EngineState.hpp
		Lilliputian.hpp
		Logger.hpp
		OSWindow.cpp
		OSWindow.hpp
		
		Profiler.hpp
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
		"Game/Scene/Camera2D.hpp"
		Engines/RenderingEngine/RenderingSDL/Rect.hpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.cpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.hpp
		Engines/RenderingEngine/RenderingSDL/SDLRenderable.hpp
		"Game/Scene/Sprite2D.hpp"
		Engines/RenderingEngine/RenderingSDL/Texture.hpp
		Engines/RenderingEngine/RenderingSDL/Transform2D.hpp
		Engines/RenderingEngine/RenderingSDL/Vector2.hpp
		"Game/Scene/Entity2D.cpp"
		"Game/Scene/Entity2D.hpp"
		Game/Game.hpp
		Game/Game.cpp
		"Game/Scene/Scene.cpp"
		"Game/Scene/Scene.hpp"
		"Game/Scene/SceneLayer2D.hpp"
		"Game/Scene/SceneLayer2D.cpp"
		"Game/Scene/Script.cpp"
		"Game/Scene/Script.hpp"
		Game/ScriptRegistry.cpp
		Game/ScriptRegistry.hpp	
		UserAPIs/GameAPI.cpp
		UserAPIs/GameAPI.hpp
		UserAPIs/InputAPI.cpp
		UserAPIs/InputAPI.hpp
		UserAPIs/ScriptingAPI.hpp
		UserAPIs/ScriptingAPI.cpp
		Utilities/Constants.hpp
		Utilities/Aliases.hpp
		Utilities/AABB2D.hpp
		Utilities/Colour.cpp
		Utilities//Colour.hpp
		Utilities/HiResTimer.hpp
		Utilities/HiResTimer.cpp
		Utilities/Rectangle.hpp
		Utilities/String.hpp
		Utilities/Transform2D.hpp
		Utilities/Vector2.hpp
		Utilities/Collections/Map.hpp
		Utilities/Collections/Queue.hpp
		Utilities/Collections/Stack.hpp
		Utilities/Collections/Vector.hpp
)

# Add source to this project's executable.
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES} "Game/ScriptRegistry.hpp" "Game/SceneLoader.hpp" "Game/SceneLoader.cpp" "FileSystem.hpp" "FileSystem.cpp" "Game/Scene/Camera2D.hpp" "Game/Scene/RigidBody2D.hpp" "Game/Scene/AnimatedSprite2D.hpp" "Game/Scene/BoxCollider2D.hpp"  "Game/Scene/AudioListener2D.hpp" "Game/Scene/StreamedAudioSource2D.hpp"  "Game/Scene/Assets/Texture.hpp" "Game/Scene/Assets/StreamedAudio.hpp" "Game/Scene/Assets/StreamedAudio.cpp" "Game/Scene/AudioListener2D.cpp" "Game/Scene/StreamedAudioSource2D.cpp" "Utilities/Collections/Set.hpp" "Game/Scene/BoxCollider2D.cpp" "Game/Scene/Camera2D.cpp" "Game/Scene/RigidBody2D.cpp" "Game/Scene/AnimatedSprite2D.cpp" "Game/Scene/Assets/Texture.cpp" "Game/Scene/Assets/Font.hpp" "Game/Scene/Assets/Font.cpp" "Game/Scene/PropertyAnimation.hpp" "Game/Scene/PropertyAnimation.cpp" "Game/Scene/Assets/Assets.cpp" "Game/Scene/Assets/Assets.hpp" "OSWindowInputs.cpp"  "OSWindowInputs.hpp" "Game/Scene/CountdownTimer.hpp" "Game/Scene/CountdownTimer.cpp" "Game/Scene/SynthesizedAudioSource2D.hpp" "Game/Scene/SynthesizedAudioSource2D.cpp" "Game/Scene/TextLabel2D.hpp" "Game/Scene/TextLabel2D.cpp" "Game/Scene/ItemList.hpp" "Game/Scene/ItemList.cpp" "Game/Scene/CircularItemList.hpp" "Game/Scene/CircularItemList.cpp" "Game/Scene/TexturedProgressBar.hpp" "Game/Scene/TexturedProgressBar.cpp" "Game/Scene/BoxTriggerArea2D.hpp" "Game/Scene/BoxTriggerArea2D.cpp" "Game/Scene/NavigationMesh2D.hpp" "Game/Scene/NavigationMesh2D.cpp" "Game/Scene/NavigationPath2D.hpp" "Game/Scene/NavigationPath2D.cpp" "Game/Scene/Sprite2D.hpp" "Game/Scene/Sprite2D.cpp" "Game/Scene/Assets/SynthesizedAudio.hpp" "Game/Scene/Assets/SynthesizedAudio.cpp" "Game/Scene/CircleCollider2D.hpp" "Game/Scene/CircleCollider2D.cpp" "Game/Scene/Assets/Text.hpp" "Game/Scene/Assets/Text.cpp" "Utilities/Image.hpp" "Utilities/Image.cpp" "UserAPIs/SceneAPI.hpp" "UserAPIs/SceneAPI.cpp" "UserAPIs/EngineAPI.hpp" "UserAPIs/EngineAPI.cpp" "Game/Scene/RemoteTransform2D.hpp" "Game/Scene/RemoteTransform2D.cpp")

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../ThirdParty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../ThirdParty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../ThirdParty/SDL2_mixer-2.0.4/lib/x64)
find_library(YAML_CPP libyaml-cppmdd.lib PATHS ../ThirdParty/yaml-cpp-0.6.3/lib/x64)

include_directories(
		../ThirdParty/SDL2-2.0.12/include
		../ThirdParty/SDL2_image-2.0.5/include
		../ThirdParty/SDL2_mixer-2.0.4/include
		../ThirdParty/yaml-cpp-0.6.3/include
)

target_link_libraries (Lilliputian
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		${YAML_CPP}
)