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
#include <os/os.hpp>

void Lilliputian::EventBus::publish(
	std::string name,
	std::vector<float> floats,
	std::vector<std::string> strings)
{
	Event::Parameters parameters = {floats, strings};
	this->publishWithParameters(name, parameters);
}

void Lilliputian::EventBus::publish(
	std::string name,
	std::vector<float> floats)
{
	Event::Parameters parameters;
	parameters.floats = floats;
	this->publishWithParameters(name, parameters);
}

void Lilliputian::EventBus::publish(
	std::string name,
	std::vector<std::string> strings)
{
	Event::Parameters parameters;
	parameters.strings = strings;
	this->publishWithParameters(name, parameters);
}

void Lilliputian::EventBus::publish(
	std::string name)
{
	Vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->events.count(name))
		eventsList = this->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDelta_ns()));

	this->events.emplace(name, eventsList);
}

void Lilliputian::EventBus::clear()
{
	this->events.clear();
}

std::vector<Lilliputian::Event> Lilliputian::EventBus::query(String name)
{
	std::vector<Event> queryResults;

	if (this->events.count(name))
		queryResults = this->events.at(name);

	return queryResults;
}

void Lilliputian::EventBus::publishWithParameters(String name, Event::Parameters parameters)
{
	Vector<Event> eventsList;

	OS::getRunTimer().setEnd();

	if (this->events.count(name))
		eventsList = this->events.at(name);

	eventsList.push_back(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		parameters));

	this->events.emplace(name, eventsList);
}