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
#include "foundations/singletons/configuration.hpp"
#include "foundations/singletons/scene_storage.hpp"
#include "systems/system.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include "foundations/singletons/profiler.hpp"
#include <memory>

#include <stdint.h>
#include "scene/scene.hpp"
#include "foundations/singletons/platform/file_access.hpp"
#include <stack>
#include "customization/class_registry.hpp"

#include "engine_api.hpp"

namespace Omnific
{
	/* The main class responsible for engine runtime 
	   that supports the application built on it. */
	class OMNIFIC_ENGINE_API Engine
	{
	public:
		Engine(int argc, char* argv[]);

		void add_app_data_directories(std::vector<std::string> app_data_directories);

		void run();
	private:
		enum class State
		{
			INITIALIZING,
			RUNNING,
			RESTARTING,
			FINALIZING
		};

		State state;
		std::unordered_map<std::string, std::shared_ptr<System>> systems;

		void initialize();
		void detect_input();
		void run_loop();
		void run_loop_on_thread();
		void sleep_for(uint32_t target_fps, std::shared_ptr<Clock> run_clock);
		void finalize();
	};
}

