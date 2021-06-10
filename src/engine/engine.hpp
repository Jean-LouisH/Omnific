// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <vector>
#include <string>
#include "game/game.hpp"
#include "systems/ai_system/ai_system.hpp"
#include "systems/animation_system/animation_system.hpp"
#include "systems/audio_system/audio_system.hpp"
#include "systems/haptic_system/haptic_system.hpp"
#include "systems/physics_system/physics_system.hpp"
#include "systems/rendering_system/rendering_system.hpp"
#include "systems/ui_system/ui_system.hpp"
#include "os/os.hpp"
#include "engine_state.hpp"
#include "os/profiler.hpp"

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

