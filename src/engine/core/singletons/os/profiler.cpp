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
#include "core/utilities/constants.hpp"
#include <core/singletons/os/os.hpp>

uint64_t Omnia::HiResTimer::get_delta_in_nanoseconds()
{
	return this->delta;
}

uint64_t Omnia::HiResTimer::get_delta()
{
	return this->delta / NS_IN_MS;
}

float Omnia::HiResTimer::get_delta_in_seconds()
{
	return this->delta / NS_IN_S;
}

void Omnia::HiResTimer::set_start()
{
	this->start = std::chrono::high_resolution_clock::now();
}

void Omnia::HiResTimer::set_end()
{
	this->end = std::chrono::high_resolution_clock::now();
	this->delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void Omnia::Profiler::add_timer(std::string timer_name, bool is_removable)
{
	this->timers.emplace(timer_name, std::shared_ptr<HiResTimer>(new HiResTimer()));
	this->is_removable_map.emplace(timer_name, is_removable);
	OS::get_logger().write("Added HiResTimer to Profiler: \"" + timer_name + "\"");
}

void Omnia::Profiler::remove_timer(std::string timer_name)
{
	if (this->is_removable_map.count(timer_name))
	{
		if (this->is_removable_map.at(timer_name))
		{
			this->timers.erase(timer_name);
			this->is_removable_map.erase(timer_name);
		}
	}
}

std::shared_ptr<Omnia::HiResTimer> Omnia::Profiler::get_timer(std::string timer_name)
{
	std::shared_ptr<HiResTimer> timer;

	if (this->timers.count(timer_name) > 0)
		timer = this->timers.at(timer_name);

	return timer;
}

void Omnia::Profiler::increment_frame_count()
{
	this->frame_count++;
}

void Omnia::Profiler::increment_lag_count(uint64_t delta_time_ms)
{
	this->lag += delta_time_ms;
}

void Omnia::Profiler::decrement_lag_count(uint64_t delta_time_ms)
{
	this->lag -= delta_time_ms;
}

uint64_t Omnia::Profiler::get_lag_count()
{
	return this->lag;
}

uint16_t Omnia::Profiler::get_fps()
{
	return (1.0 / (this->get_timer("frame")->get_delta_in_nanoseconds() / NS_IN_S));
}