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
#include "application/application.hpp"
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
#include <memory>

#if defined (_WIN32)
#if defined(_OMNIA_ENGINE_EXPORTS)
#define  OMNIA_ENGINE_API __declspec(dllexport)
#else
#define  OMNIA_ENGINE_API __declspec(dllimport)
#endif
#else
#define OMNIA_ENGINE_API
#endif

namespace Omnia
{
	/* The main class responsible for engine runtime 
	   that supports the application built on it. */
	class OMNIA_ENGINE_API Engine
	{
	public:
		Engine(
			int argc,
			char* argv[]);

		void run();
	private:
		std::unique_ptr<Application> application;
		std::unique_ptr<AISystem> aiSystem;
		std::unique_ptr<AnimationSystem> animationSystem;
		std::unique_ptr<AudioSystem> audioSystem;
		std::unique_ptr<HapticSystem> hapticSystem;
		std::unique_ptr<PhysicsSystem> physicsSystem;
		std::unique_ptr<RenderingSystem> renderingSystem;
		std::unique_ptr<UISystem> uiSystem;
		std::unique_ptr<EngineState> state;

		int argc = 0;
		char** argv = nullptr;

		/* Returns "true" if initialization is successful */
		bool initialize();
		void input();
		void update();
		void output();
		void benchmark();
		/* For the current single-threaded engine 
		   loop implementation*/
		void sleep();
		void shutdown();
	};
}

