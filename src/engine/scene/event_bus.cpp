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

#include "event_bus.hpp"
#include <singletons/os/os.hpp>

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers,
	std::unordered_map<std::string, std::string> strings)
{
	Event::Parameters parameters = {numbers, strings};
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, double> numbers)
{
	Event::Parameters parameters;
	parameters.numbers = numbers;
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name,
	std::unordered_map<std::string, std::string> strings)
{
	Event::Parameters parameters;
	parameters.strings = strings;
	this->publishWithParameters(name, parameters);
}

void Omnia::EventBus::publish(
	std::string name)
{
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->events.count(name))
		eventsList = this->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds()));

	this->events.emplace(name, eventsList);
}

void Omnia::EventBus::clear()
{
	this->events.clear();
}

std::vector<Omnia::Event> Omnia::EventBus::query(std::string name)
{
	std::vector<Event> queryResults;

	if (this->events.count(name))
		queryResults = this->events.at(name);

	return queryResults;
}

void Omnia::EventBus::publishWithParameters(std::string name, Event::Parameters parameters)
{
	std::vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->events.count(name))
		eventsList = this->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDeltaInNanoseconds(),
		parameters));

	if (this->events.count(name))
		this->events.at(name) = eventsList;
	else
		this->events.emplace(name, eventsList);
}