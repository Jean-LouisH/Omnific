#pragma once

#include "Utilities/GenericCollections/Vector.hpp"
#include "Game/Game.hpp"
#include "Engines/AIEngine/AIEngine.hpp"
#include "Engines/AnimationEngine/AnimationEngine.hpp"
#include "Engines/AudioEngine/AudioEngine.hpp"
#include "Engines/HapticEngine/HapticEngine.hpp"
#include "Engines/PhysicsEngine/PhysicsEngine.hpp"
#include "Engines/RenderingEngine/RenderingEngine.hpp"
#include "OSWindow.hpp"
#include "Platform.hpp"
#include "EngineState.hpp"

namespace Lilliputian
{
	using SceneDefinerCallback = void(*)(Lilliputian::EditorAPI*);

	class Engine
	{
	private:
		Vector<SceneDefinerCallback> sceneDefinerCallbacks;
		Game* game;

		AIEngine* aiEngine;
		AnimationEngine* animationEngine;
		AudioEngine* audioEngine;
		HapticEngine* hapticEngine;
		PhysicsEngine* physicsEngine;
		RenderingEngine* renderingEngine;

		OSWindow* osWindow;
		Platform* platform;

		uint64_t frameCount;
		uint16_t FPS;
		EngineState state;

		uint32_t msPerComputeUpdate;
		uint32_t targetFPS;

		void initialize();
		void shutdown();
		void sleep();
		void benchmark();
		void input();
		void logic();
		void compute();
		void output();
	public:
		Engine();
		~Engine();

		void run();
		void setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate);
		void setTargetFPS(uint32_t targetFPS);
		void addSceneDefiner(SceneDefinerCallback sceneInitializerCallback);
	};
}

