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

#include "profiler.hpp"
#include "foundations/constants.hpp"
#include <foundations/singletons/platform/platform.hpp>

Omnific::Profiler* Omnific::Profiler::instance = nullptr;

void Omnific::Profiler::add_clock(std::string clock_name, bool is_removable)
{
	Profiler* instance = Profiler::get_instance();
	std::shared_ptr<Clock> clock = std::shared_ptr<Clock>(new Clock());
	clock->set_start();
	instance->clocks.emplace(clock_name, clock);
	instance->is_removable_map.emplace(clock_name, is_removable);
	Platform::get_logger().write("Added HiResTimer to Profiler: \"" + clock_name + "\"");
}

void Omnific::Profiler::remove_clock(std::string clock_name)
{
	Profiler* instance = Profiler::get_instance();

	if (instance->is_removable_map.count(clock_name))
	{
		if (instance->is_removable_map.at(clock_name))
		{
			instance->clocks.erase(clock_name);
			instance->is_removable_map.erase(clock_name);
		}
	}
}

std::shared_ptr<Omnific::Clock> Omnific::Profiler::get_clock(std::string clock_name)
{
	std::shared_ptr<Clock> clock;
	Profiler* instance = Profiler::get_instance();

	if (instance->clocks.count(clock_name) > 0)
		clock = instance->clocks.at(clock_name);

	return clock;
}

void Omnific::Profiler::increment_frame_count()
{
	Profiler::get_instance()->frame_count++;
}

void Omnific::Profiler::increment_lag_count(uint64_t delta_time_ms)
{
	Profiler::get_instance()->lag += delta_time_ms;
}

void Omnific::Profiler::decrement_lag_count(uint64_t delta_time_ms)
{
	Profiler::get_instance()->lag -= delta_time_ms;
}

uint64_t Omnific::Profiler::get_lag_count()
{
	return Profiler::get_instance()->lag;
}

uint16_t Omnific::Profiler::get_fps()
{
	return (1.0 / (Profiler::get_instance()->get_clock("frame")->get_delta_in_seconds()));
}

Omnific::Profiler* Omnific::Profiler::get_instance()
{
	if (instance == nullptr)
		instance = new Profiler();
	return instance;
}