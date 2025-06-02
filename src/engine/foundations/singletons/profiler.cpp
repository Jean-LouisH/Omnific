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

void Omnific::Profiler::add_clock(std::string clock_name)
{
	Profiler* instance = Profiler::get_instance();
	instance->add_clock(clock_name, {}, true);
}

void Omnific::Profiler::add_clock(std::string clock_name, std::vector<std::string> tags, bool is_removable)
{
	Profiler* instance = Profiler::get_instance();
	std::shared_ptr<Clock> clock = std::shared_ptr<Clock>(new Clock(clock_name));
	clock->set_start();
	instance->clocks.emplace(clock_name, clock);
	for (std::string tag: tags)
	{
		if (instance->clock_names_by_tags.count(tag) > 0)
		{
			instance->clock_names_by_tags.at(tag).push_back(clock_name);
		}
		else
		{
			std::vector<std::string> clock_names;
			clock_names.push_back(clock_name);
			instance->clock_names_by_tags.emplace(tag, clock_names);
		}
	}
	instance->is_removable_map.emplace(clock_name, is_removable);
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

std::vector<std::shared_ptr<Omnific::Clock>> Omnific::Profiler::get_clocks_by_tag(std::string tag)
{
	std::vector<std::shared_ptr<Clock>> clocks_by_tag;
	Profiler* instance = Profiler::get_instance();

	if (instance->clock_names_by_tags.count(tag))
	{
		for (auto clock_name : instance->clock_names_by_tags.at(tag))
		{
			if (instance->clocks.count(clock_name) > 0)
			{
				clocks_by_tag.push_back(instance->clocks.at(clock_name));
			}
		}
	}

	return clocks_by_tag;
}

std::string Omnific::Profiler::get_clock_deltas_to_string_by_tag(std::string tag)
{
	std::string clock_deltas_string;
	Profiler* instance = Profiler::get_instance();
	std::vector<std::shared_ptr<Clock>> clocks_by_tag = instance->get_clocks_by_tag(tag);

	for (std::shared_ptr<Clock> clock: clocks_by_tag)
	{
		clock_deltas_string += clock->get_name() + ": " + std::to_string(clock->get_delta()) + " ms\n"; 
	}

	return clock_deltas_string;
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

Omnific::Profiler* Omnific::Profiler::get_instance()
{
	if (instance == nullptr)
		instance = new Profiler();
	return instance;
}