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

#include "event_queue.hpp"
#include <os/os.hpp>

void Lilliputian::EventQueue::enqueue(
	std::string name,
	Event::Priority priority,
	std::vector<float> floats,
	std::vector<std::string> strings)
{
	Event::Parameters parameters;

	parameters.floats = floats;
	parameters.strings = strings;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		priority,
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	Event::Priority priority,
	std::vector<float> floats)
{
	Event::Parameters parameters;

	parameters.floats = floats;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		priority,
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	Event::Priority priority,
	std::vector<std::string> strings)
{
	Event::Parameters parameters;

	parameters.strings = strings;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		priority,
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	Event::Priority priority)
{
	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		priority));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	std::vector<float> floats,
	std::vector<std::string> strings)
{
	Event::Parameters parameters;

	parameters.floats = floats;
	parameters.strings = strings;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	std::vector<float> floats)
{
	Event::Parameters parameters;

	parameters.floats = floats;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name,
	std::vector<std::string> strings)
{
	Event::Parameters parameters;

	parameters.strings = strings;

	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name,
		OS::getRunTimer().getDelta_ns(),
		parameters));
}

void Lilliputian::EventQueue::enqueue(
	std::string name)
{
	OS::getRunTimer().setEnd();
	this->events.emplace(Event(
		name, 
		OS::getRunTimer().getDelta_ns()));
}

Lilliputian::Event Lilliputian::EventQueue::dequeue()
{
	Event frontEvent = this->events.front();
	this->events.pop();
	return frontEvent;
}

bool Lilliputian::EventQueue::isEmpty()
{
	return this->events.empty();
}

Lilliputian::Event& Lilliputian::EventQueue::peek()
{
	return this->events.front();
}