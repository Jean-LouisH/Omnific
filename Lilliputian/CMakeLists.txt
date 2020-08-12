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
		Platform.hpp
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
		Engines/RenderingEngine/RenderingSDL/Camera2D.hpp
		Engines/RenderingEngine/RenderingSDL/Rect.hpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.cpp
		Engines/RenderingEngine/RenderingSDL/RenderingSDL.hpp
		Engines/RenderingEngine/RenderingSDL/SDLRenderable.hpp
		Engines/RenderingEngine/RenderingSDL/Sprite2D.hpp
		Engines/RenderingEngine/RenderingSDL/Texture.hpp
		Engines/RenderingEngine/RenderingSDL/Transform2D.hpp
		Engines/RenderingEngine/RenderingSDL/Vector2.hpp
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
add_library(Lilliputian STATIC ${LILLIPUTIAN_SOURCES} "Game/ScriptRegistry.hpp" "Game/SceneLoader.hpp" "Game/SceneLoader.cpp" "FileSystem.hpp" "FileSystem.cpp" "Game/Camera2D.hpp" "Game/RigidBody2D.hpp" "Game/AnimatedSprite2D.hpp" "Game/BoxCollider2D.hpp"  "Game/AudioListener2D.hpp" "Game/AudioSource2D.hpp"  "Game/Texture.hpp" "Game/Audio.hpp" "Game/Audio.cpp" "Game/AudioListener2D.cpp" "Game/AudioSource2D.cpp" "Utilities/Collections/Set.hpp" "Game/BoxCollider2D.cpp" "Game/Camera2D.cpp" "Game/RigidBody2D.cpp" "Game/AnimatedSprite2D.cpp" "Game/Texture.cpp" "Game/Font.hpp" "Game/Font.cpp" "Game/PropertyAnimation.hpp" "Game/PropertyAnimation.cpp" "Game/Assets.cpp" "Game/Assets.hpp" "OSWindowInputs.cpp"  "OSWindowInputs.hpp" "Game/CountdownTimer.hpp" "Game/CountdownTimer.cpp" "Game/ProceduralAudioSource2D.hpp" "Game/ProceduralAudioSource2D.cpp" "Game/TextLabel.hpp" "Game/TextLabel.cpp" "Game/ItemList.hpp" "Game/ItemList.cpp" "Game/CircularItemList.hpp" "Game/CircularItemList.cpp" "Game/TexturedProgressBar.hpp" "Game/TexturedProgressBar.cpp" "Game/BoxTriggerArea2D.hpp" "Game/BoxTriggerArea2D.cpp" "Game/NavigationMesh2D.hpp" "Game/NavigationMesh2D.cpp" "Game/NavigationPath2D.hpp" "Game/NavigationPath2D.cpp" "Game/Sprite2D.hpp" "Game/Sprite2D.cpp")

find_library(SDL2 SDL2.lib SDL2main.lib PATHS ../ThirdParty/SDL2-2.0.12/lib/x64)
find_library(SDL2_Image SDL2_image.lib PATHS ../ThirdParty/SDL2_image-2.0.5/lib/x64)
find_library(SDL2_Mixer SDL2_mixer.lib PATHS ../ThirdParty/SDL2_mixer-2.0.4/lib/x64)

include_directories(
		../ThirdParty/SDL2-2.0.12/include
		../ThirdParty/SDL2_image-2.0.5/include
		../ThirdParty/SDL2_mixer-2.0.4/include
		../ThirdParty/yaml-cpp-0.6.3/include
)

add_dependencies(Lilliputian
		yaml-cpp)

target_link_libraries (Lilliputian
		${SDL2}
		${SDL2_Image}
		${SDL2_Mixer}
		yaml-cpp
)