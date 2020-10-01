#pragma once

#include "Utilities/Collections/Vector.hpp"
#include "Utilities/String.hpp"
#include "Game/Game.hpp"
#include "Game/ScriptRegistry.hpp"
#include "Engines/AIEngine/AIEngine.hpp"
#include "Engines/AnimationEngine/AnimationEngine.hpp"
#include "Engines/AudioEngine/AudioEngine.hpp"
#include "Engines/HapticEngine/HapticEngine.hpp"
#include "Engines/PhysicsEngine/PhysicsEngine.hpp"
#include "Engines/RenderingEngine/RenderingEngine.hpp"
#include "OS.hpp"
#include "EngineConfiguration.hpp"
#include "EngineState.hpp"
#include "Profiler.hpp"
#include "ThreadPool.hpp"

namespace Lilliputian
{
	using ScriptCompilerCallback = void(*)(Lilliputian::ScriptRegistry&);

	class Engine
	{
	private:
		Vector<ScriptCompilerCallback> scriptCompilerCallbacks;
		Game* game = NULL;
		AIEngine* aiEngine = NULL;
		AnimationEngine* animationEngine = NULL;
		AudioEngine* audioEngine = NULL;
		HapticEngine* hapticEngine = NULL;
		PhysicsEngine* physicsEngine = NULL;
		RenderingEngine* renderingEngine = NULL;
		OS* os = NULL;
		Profiler* profiler = NULL;
		ThreadPool* threadPool = NULL;
		EngineConfiguration configuration;
		EngineState state;

		char** commandArguments = nullptr;

		void setGameTitle(const char* gameTitle);
		void setEntryScene(const char* entryScenePath);
		void setWindowDimensions(uint16_t width, uint16_t height);

		void initialize();
		void input();
		void logic();
		void compute();
		void output();
		void sleep();
		void shutdown();
	public:
		Engine(
			int argc,
			char* argv[], 
			const char* gameTitle, 
			uint16_t windowWidth, 
			uint16_t windowHeight, 
			const char* entryScenePath);
		~Engine();

		void run();
		void setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate);
		void setTargetFPS(uint32_t targetFPS);
		void setFullscreenStart(bool value);
		void addScriptCompilerFunction(ScriptCompilerCallback scriptCompilerCallback);
	};
}

