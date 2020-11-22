#pragma once

#include "Utilities/Collections/Vector.hpp"
#include "Utilities/String.hpp"
#include "Game/Game.hpp"
#include "Game/ScriptRegistry.hpp"
#include "Systems/AISystem/AISystem.hpp"
#include "Systems/AnimationSystem/AnimationSystem.hpp"
#include "Systems/AudioSystem/AudioSystem.hpp"
#include "Systems/HapticSystem/HapticSystem.hpp"
#include "Systems/PhysicsSystem/PhysicsSystem.hpp"
#include "Systems/RenderingSystem/RenderingSystem.hpp"
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
	private:
		Vector<ScriptCompilerCallback> scriptCompilerCallbacks;
		Game* game = NULL;
		AISystem* aiSystem = NULL;
		AnimationSystem* animationSystem = NULL;
		AudioSystem* audioSystem = NULL;
		HapticSystem* hapticSystem = NULL;
		PhysicsSystem* physicsSystem = NULL;
		RenderingSystem* renderingSystem = NULL;
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
	};
}

