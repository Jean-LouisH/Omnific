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

#include "foundations/singletons/event_bus.hpp"
#include "platform/platform.hpp"

Omnific::EventBus* Omnific::EventBus::instance = nullptr;

Omnific::Event::Event(std::string name, Parameters parameters)
{
	this->name = name;
	Platform::get_run_clock().set_end();
	this->timestamp = Platform::get_run_clock().get_delta();
	this->parameters = parameters;
	if (this->parameters.key == "")
		this->parameters.key = name + "_" + std::to_string(this->timestamp);
}

Omnific::Event::Event(std::string name)
{
	this->name = name;
	Platform::get_run_clock().set_end();
	this->timestamp = Platform::get_run_clock().get_delta();
	this->parameters.key = name + "_" + std::to_string(this->timestamp);
}

Omnific::Event::Event()
{

}

std::string Omnific::Event::get_name()
{
	return this->name;
}

uint64_t Omnific::Event::get_timestamp()
{
	return this->timestamp;
}

Omnific::Event::Parameters Omnific::Event::get_parameters()
{
	return this->parameters;
}

void Omnific::EventBus::publish_event(
	Event event,
	bool is_continuous)
{
	EventBus::publish_with_parameters(event, is_continuous);
}

void Omnific::EventBus::publish_event(
	std::string name,
	std::unordered_map<std::string, std::string> strings,
	std::unordered_map<std::string, double> numbers,
	std::unordered_map<std::string, bool> booleans,
	std::unordered_map<std::string, std::shared_ptr<Component>> components,
	std::string key,
	bool is_continuous)	
{
	Event::Parameters parameters = {numbers, strings, booleans, components, key};
	EventBus::publish_with_parameters(Event(name, parameters), is_continuous);
}

void Omnific::EventBus::clear_instant_events()
{
	EventBus::get_instance()->instant_events.clear();
}

void Omnific::EventBus::clear_continuous_events()
{
	EventBus::get_instance()->continuous_events.clear();
}

void Omnific::EventBus::remove_continuous_event(std::string event_name, std::string event_key)
{
	EventBus* event_bus = EventBus::get_instance();
	std::unordered_map<std::string, Event>& event_map = EventBus::get_instance()->continuous_events.at(event_name);
	event_map.erase(event_key);
}

std::vector<Omnific::Event> Omnific::EventBus::query_events(std::string event_name)
{
	EventBus* event_bus = EventBus::get_instance();
	std::vector<Event> query_results;

	if (event_bus->instant_events.count(event_name))
	{
		query_results = event_bus->instant_events.at(event_name);
	}
	else if (event_bus->continuous_events.count(event_name))
	{
		for (auto event_pair: event_bus->continuous_events.at(event_name))
		{
			query_results.push_back(event_pair.second);
		}
	}

	return query_results;
}

std::unordered_map<std::string, Omnific::Event> Omnific::EventBus::query_continuous_events(std::string event_name)
{
	EventBus* event_bus = EventBus::get_instance();
	std::unordered_map<std::string, Event> query_results;

	if (event_bus->continuous_events.count(event_name))
		query_results = event_bus->continuous_events.at(event_name);

	return query_results;
}

Omnific::Event Omnific::EventBus::query_continuous_event(std::string event_name, std::string event_key)
{
	Event continuous_event;
	std::unordered_map<std::string, std::unordered_map<std::string, Event>> continuous_events = EventBus::get_instance()->continuous_events;

	if (continuous_events.count(event_name))
		if (continuous_events.at(event_name).count(event_key))
			continuous_event = continuous_events.at(event_name).at(event_key);

	return continuous_event;
}

std::vector<double> Omnific::EventBus::get_numbers(std::string event_name, std::string event_parameter_key)
{
	std::vector<double> matched_numbers;

	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.numbers.count(event_parameter_key))
		{
			matched_numbers.push_back(parameter.numbers.at(event_parameter_key));
		}
	}
	
	return matched_numbers;
}

std::vector<std::string> Omnific::EventBus::get_strings(std::string event_name, std::string event_parameter_key)
{
	std::vector<std::string> matched_strings;

	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.strings.count(event_parameter_key))
		{
			matched_strings.push_back(parameter.strings.at(event_parameter_key));
		}
	}
	
	return matched_strings;
}

std::vector<bool> Omnific::EventBus::get_bools(std::string event_name, std::string event_parameter_key)
{
	std::vector<bool> matched_bools;

	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.bools.count(event_parameter_key))
		{
			matched_bools.push_back(parameter.bools.at(event_parameter_key));
		}
	}
	
	return matched_bools;
}

std::vector<std::shared_ptr<Omnific::Component>> Omnific::EventBus::get_components(std::string event_name, std::string event_parameter_key)
{
	std::vector<std::shared_ptr<Component>> matched_components;

	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.components.count(event_parameter_key))
		{
			matched_components.push_back(parameter.components.at(event_parameter_key));
		}
	}
	
	return matched_components;
}

std::vector<Omnific::Event> Omnific::EventBus::query_events_with_number_parameter(std::string event_name, std::string event_parameter_key, double parameter_value)
{
	std::vector<Event> matched_events;
	const double epsilon = 0.1;
	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.numbers.count(event_parameter_key))
		{
			double value = parameter.numbers.at(event_parameter_key);
			if (value + epsilon > parameter_value && parameter_value > value - epsilon)
			{
				matched_events.push_back(event);
			}
		}
	}
	return matched_events;
}

std::vector<Omnific::Event> Omnific::EventBus::query_events_with_string_parameter(std::string event_name, std::string event_parameter_key, std::string parameter_value)
{
	std::vector<Event> matched_events;
	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.strings.count(event_parameter_key))
		{
			if (parameter.strings.at(event_parameter_key) == parameter_value)
			{
				matched_events.push_back(event);
			}
		}
	}
	return matched_events;
}

std::vector<Omnific::Event> Omnific::EventBus::query_events_with_bool_parameter(std::string event_name, std::string event_parameter_key, bool parameter_value)
{
	std::vector<Event> matched_events;
	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.bools.count(event_parameter_key))
		{
			if (parameter.bools.at(event_parameter_key) == parameter_value)
			{
				matched_events.push_back(event);
			}
		}
	}
	return matched_events;
}

std::vector<Omnific::Event> Omnific::EventBus::query_events_with_component_parameter(std::string event_name, std::string event_parameter_key, std::shared_ptr<Component> parameter_value)
{
	std::vector<Event> matched_events;
	for (Event event: EventBus::query_events(event_name))
	{
		Event::Parameters parameter = event.get_parameters();
		if (parameter.components.count(event_parameter_key))
		{
			if (parameter.components.at(event_parameter_key)->get_id() == parameter_value->get_id())
			{
				matched_events.push_back(event);
			}
		}
	}
	return matched_events;
}

bool Omnific::EventBus::has_continuous_event(std::string event_name, std::string event_key)
{
	Event continuous_event = EventBus::get_instance()->query_continuous_event(event_name, event_key);
	return continuous_event.get_name() != "";
}

uint64_t Omnific::EventBus::query_event_count(std::string event_name)
{
	return EventBus::get_instance()->query_events(event_name).size();
}

uint64_t Omnific::EventBus::query_event_count_with_parameter_key(std::string event_name, std::string event_parameter_name)
{
	uint64_t event_count = 0;
	std::vector<Event> event_list = EventBus::query_events((event_name));
	for (size_t i = 0; i < event_list.size(); ++i)
	{
		Event::Parameters parameters = event_list.at(i).get_parameters();
		event_count += parameters.bools.count(event_parameter_name);
		event_count += parameters.numbers.count(event_parameter_name);
		event_count += parameters.strings.count(event_parameter_name);
		event_count += parameters.components.count(event_parameter_name);
	}
	return event_count;
}

bool Omnific::EventBus::has_event(std::string event_name)
{
	return EventBus::query_event_count(event_name) > 0;
}

bool Omnific::EventBus::has_event_with_parameter_key(std::string event_name, std::string event_parameter_key)
{
	return EventBus::query_event_count_with_parameter_key(event_name, event_parameter_key) > 0;
}

void Omnific::EventBus::publish_with_parameters(
	Event event,
	bool is_continuous)
{
	EventBus* event_bus = EventBus::get_instance();
	std::string event_name = event.get_name();

	if (!is_continuous)
	{
		if (event_bus->instant_events.count(event_name))
		{
			event_bus->instant_events.at(event_name).push_back(event);
		}
		else
		{
			std::vector<Event> event_list;
			event_list.push_back(event);
			event_bus->instant_events.emplace(event_name, event_list);
		}
	}
	else
	{
		std::string event_key = event.get_parameters().key;

		if (event_bus->continuous_events.count(event_name))
		{
			event_bus->continuous_events.at(event_name).emplace(event_key, event);
		}
		else
		{
			std::unordered_map<std::string, Event> event_dictionary;
			event_dictionary.emplace(event_key, event);
			event_bus->continuous_events.emplace(event_name, event_dictionary);
		}
	}
}

Omnific::EventBus* Omnific::EventBus::get_instance()
{
	if (instance == nullptr)
	{
		instance = new EventBus();
		const size_t reservation_amount = 64;
		instance->instant_events.reserve(reservation_amount);
		instance->continuous_events.reserve(reservation_amount);
	}
	return instance;
}