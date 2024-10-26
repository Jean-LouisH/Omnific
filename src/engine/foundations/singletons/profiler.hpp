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
#include <foundations/singletons/platform/platform.hpp>
#include <engine_api.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Profiler
	{
	public:
		static void add_clock(std::string clock_name, bool is_removable = false);
		static void remove_clock(std::string clock_name);
		static std::shared_ptr<Clock> get_clock(std::string clock_name);

		static void increment_frame_count();
		static void increment_lag_count(uint64_t delta_time);
		static void decrement_lag_count(uint64_t delta_time);
		/* In milliseconds */
		static uint64_t get_lag_count();
	private:
		std::unordered_map<std::string, std::shared_ptr<Clock>> clocks;
		std::unordered_map<std::string, bool> is_removable_map;
		/* In milliseconds */
		uint64_t lag = 0;
		uint64_t frame_count = 0;

		static Profiler* instance;
		static Profiler* get_instance();
	};
}