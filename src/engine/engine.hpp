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
#include "core/singletons/configuration.hpp"
#include "core/singletons/scene_storage.hpp"
#include "core/system.hpp"
#include "core/singletons/os/os.hpp"
#include "core/singletons/os/profiler.hpp"
#include <memory>

#include <stdint.h>
#include "core/scene/scene.hpp"
#include "core/singletons/os/os.hpp"
#include "core/singletons/os/file_access.hpp"
#include "core/singletons/os/profiler.hpp"
#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "engine_api.hpp"

namespace Omnia
{
	/* The main class responsible for engine runtime 
	   that supports the application built on it. */
	class OMNIA_ENGINE_API Engine
	{
	public:
		void run(
			int argc,
			char* argv[]
		);
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
		void run_loop(std::shared_ptr<HiResTimer> loop_process_timer);
		void sleep_this_thread_for_remaining_time(uint32_t target_fps, std::shared_ptr<HiResTimer> run_timer);
		void finalize();

		template<class T>
		std::shared_ptr<T> get_system()
		{
			std::shared_ptr<T> system;

			if (this->systems.count(T::TYPE_STRING))
				system = std::dynamic_pointer_cast<T>(this->systems.at(T::TYPE_STRING));

			return system;
		}
	};
}

