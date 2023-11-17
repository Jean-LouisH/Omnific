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

#include "core/singletons/event_bus.hpp"
#include "os/os.hpp"

Omnia::EventBus* Omnia::EventBus::instance = nullptr;

Omnia::Event::Event(std::string name, uint64_t timestamp, Parameters parameters)
{
	this->name = name;
	this->timestamp = timestamp;
	this->parameters = parameters;
}

Omnia::Event::Event(std::string name, uint64_t timestamp)
{
	this->name = name;
	this->timestamp = timestamp;
}

std::string Omnia::Event::get_name()
{
	return this->name;
}

uint64_t Omnia::Event::get_timestamp()
{
	return this->timestamp;
}

Omnia::Event::Parameters Omnia::Event::get_parameters()
{
	return this->parameters;
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers,
	std::unordered_map<std::string, std::string> strings)
{
	EventBus* event_bus = EventBus::get_instance();
	Event::Parameters parameters = { numbers, strings };
	event_bus->publish_with_parameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers)
{
	EventBus* event_bus = EventBus::get_instance();
	Event::Parameters parameters;
	parameters.numbers = numbers;
	event_bus->publish_with_parameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, std::string> strings)
{
	EventBus* event_bus = EventBus::get_instance();
	Event::Parameters parameters;
	parameters.strings = strings;
	event_bus->publish_with_parameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name)
{
	EventBus* event_bus = EventBus::get_instance();
	std::vector<Event> events_list;

	OS::get_run_timer().set_end();

	if (event_bus->events.count(name))
		events_list = event_bus->events.at(name);

	events_list.push_back(Event(
		name,
		OS::get_run_timer().get_delta_in_nanoseconds()));

	event_bus->events.emplace(name, events_list);
}

void Omnia::EventBus::clear()
{
	EventBus* event_bus = EventBus::get_instance();
	event_bus->events.clear();
}

std::vector<Omnia::Event> Omnia::EventBus::query(std::string name)
{
	EventBus* event_bus = EventBus::get_instance();
	std::vector<Event> query_results;

	if (event_bus->events.count(name))
		query_results = event_bus->events.at(name);

	return query_results;
}

uint64_t Omnia::EventBus::query_count(std::string name)
{
	EventBus* event_bus = EventBus::get_instance();
	return event_bus->query(name).size();
}

void Omnia::EventBus::publish_with_parameters(std::string name, Event::Parameters parameters)
{
	EventBus* event_bus = EventBus::get_instance();
	std::vector<Event> events_list;

	OS::get_run_timer().set_end();

	if (event_bus->events.count(name))
		events_list = event_bus->events.at(name);

	events_list.push_back(Event(
		name,
		OS::get_run_timer().get_delta_in_nanoseconds(),
		parameters));

	if (event_bus->events.count(name))
	{
		event_bus->events.at(name) = events_list;
	}
	else
	{
		event_bus->events.emplace(name, events_list);
	}
}

Omnia::EventBus* Omnia::EventBus::get_instance()
{
	if (instance == nullptr)
	{
		instance = new EventBus();
		instance->events.reserve(32);
	}
	return instance;
}