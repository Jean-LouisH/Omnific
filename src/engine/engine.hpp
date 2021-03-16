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
#include "systems/ui_system/ui_system.hpp"
#include "os.hpp"
#include "engine_state.hpp"
#include "profiler.hpp"

#if defined (_WIN32)
#if defined(_LILLIPUTIAN_ENGINE_EXPORTS)
#define  LILLIPUTIAN_ENGINE_API __declspec(dllexport)
#else
#define  LILLIPUTIAN_ENGINE_API __declspec(dllimport)
#endif
#else
#define LILLIPUTIAN_ENGINE_API
#endif

namespace Lilliputian
{
	class LILLIPUTIAN_ENGINE_API Engine
	{
	public:
		Engine(
			int argc,
			char* argv[]);

		void run();
	private:
		Game* game = nullptr;
		AISystem* aiSystem = nullptr;
		AnimationSystem* animationSystem = nullptr;
		AudioSystem* audioSystem = nullptr;
		HapticSystem* hapticSystem = nullptr;
		PhysicsSystem* physicsSystem = nullptr;
		RenderingSystem* renderingSystem = nullptr;
		UISystem* uiSystem = nullptr;
		OS* os = nullptr;
		Profiler* profiler = nullptr;
		EngineState state;

		int argc = 0;
		char** argv = nullptr;

		bool initialize();
		void input();
		void update();
		void output();
		void benchmark();
		void sleep();
		void shutdown();
	};
}

