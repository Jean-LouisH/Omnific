#pragma once

#include "utilities/collections/vector.hpp"
#include "utilities/string.hpp"
#include "game/game.hpp"
#include "game/script_registry.hpp"
#include "systems/ai_system/ai_system.hpp"
#include "systems/animation_system/animation_system.hpp"
#include "systems/audio_system/audio_system.hpp"
#include "systems/haptic_system/haptic_system.hpp"
#include "systems/physics_system/physics_system.hpp"
#include "systems/rendering_system/rendering_system.hpp"
#include "os.hpp"
#include "engine_configuration.hpp"
#include "engine_state.hpp"
#include "profiler.hpp"

namespace Lilliputian
{
	using ScriptCompilerCallback = void(*)(Lilliputian::ScriptRegistry&);

	class Engine
	{
	public:
		Engine(
			int argc,
			char* argv[]);
		~Engine();

		void run();
		void setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate);
		void setTargetFPS(uint32_t targetFPS);
		void setFullscreenStart(bool value);
		void addScriptCompilerFunction(ScriptCompilerCallback scriptCompilerCallback);
	private:
		Vector<ScriptCompilerCallback> scriptCompilerCallbacks;
		Game* game = nullptr;
		AISystem* aiSystem = nullptr;
		AnimationSystem* animationSystem = nullptr;
		AudioSystem* audioSystem = nullptr;
		HapticSystem* hapticSystem = nullptr;
		PhysicsSystem* physicsSystem = nullptr;
		RenderingSystem* renderingSystem = nullptr;
		OS* os = nullptr;
		Profiler* profiler = nullptr;
		EngineConfiguration configuration;
		EngineState state;

		char** commandArguments = nullptr;

		void setGameTitle(const char* gameTitle);
		void setEntryScene(const char* entryScenePath);
		void setWindowDimensions(uint16_t width, uint16_t height);

		bool initializeSystems();
		void loadConfiguration(EngineConfiguration configuration);
		void input();
		void logic();
		void compute();
		void output();
		void sleep();
		void shutdown();
	};
}

