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

#include <stdint.h>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <string>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API HiResTimer
	{
	public:
		uint64_t get_delta_in_nanoseconds();
		/* Returns delta time in Milliseconds */
		uint64_t get_delta();
		float get_delta_in_seconds();
		void set_start();
		void set_end();
	private:
		uint64_t delta = 0; //In nanoseconds.
		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> end;
	};

	class OMNIA_ENGINE_API Profiler
	{
	public:
		void add_timer(std::string timer_name, bool is_removable= true);
		void remove_timer(std::string timer_name);
		std::shared_ptr<HiResTimer> get_timer(std::string timer_name);

		void increment_frame_count();
		void increment_lag_count(uint64_t delta_time);
		void decrement_lag_count(uint64_t delta_time);
		/* In milliseconds */
		uint64_t get_lag_count();
		uint16_t get_fps();
	private:
		std::unordered_map<std::string, std::shared_ptr<HiResTimer>> timers;
		std::unordered_map<std::string, bool> is_removable_map;
		/* In milliseconds */
		uint64_t lag = 0;
		uint64_t frame_count = 0;
	};
}