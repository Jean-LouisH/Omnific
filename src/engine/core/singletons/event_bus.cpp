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

std::string Omnia::Event::getName()
{
	return this->name;
}

uint64_t Omnia::Event::getTimestamp()
{
	return this->timestamp;
}

Omnia::Event::Parameters Omnia::Event::getParameters()
{
	return this->parameters;
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers,
	std::unordered_map<std::string, std::string> strings)
{
	EventBus* eventBus = EventBus::getInstance();
	Event::Parameters parameters = { numbers, strings };
	eventBus->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers)
{
	EventBus* eventBus = EventBus::getInstance();
	Event::Parameters parameters;
	parameters.numbers = numbers;
	eventBus->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, std::string> strings)
{
	EventBus* eventBus = EventBus::getInstance();
	Event::Parameters parameters;
	parameters.strings = strings;
	eventBus->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name)
{
	EventBus* eventBus = EventBus::getInstance();
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (eventBus->events.count(name))
		eventsList = eventBus->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds()));

	eventBus->events.emplace(name, eventsList);
}

void Omnia::EventBus::clear()
{
	EventBus* eventBus = EventBus::getInstance();
	eventBus->events.clear();
}

std::vector<Omnia::Event> Omnia::EventBus::query(std::string name)
{
	EventBus* eventBus = EventBus::getInstance();
	std::vector<Event> queryResults;

	if (eventBus->events.count(name))
		queryResults = eventBus->events.at(name);

	return queryResults;
}

uint64_t Omnia::EventBus::queryCount(std::string name)
{
	EventBus* eventBus = EventBus::getInstance();
	return eventBus->query(name).size();
}

void Omnia::EventBus::publishWithParameters(std::string name, Event::Parameters parameters)
{
	EventBus* eventBus = EventBus::getInstance();
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (eventBus->events.count(name))
		eventsList = eventBus->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds(),
		parameters));

	if (eventBus->events.count(name))
	{
		eventBus->events.at(name) = eventsList;
	}
	else
	{
		eventBus->events.emplace(name, eventsList);
	}
}

Omnia::EventBus* Omnia::EventBus::getInstance()
{
	if (instance == nullptr)
	{
		instance = new EventBus();
		instance->events.reserve(32);
	}
	return instance;
}