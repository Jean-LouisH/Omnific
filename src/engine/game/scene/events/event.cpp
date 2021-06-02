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

#include "event.hpp"

Lilliputian::Event::Event(std::string name, uint64_t timestamp, Priority priority, Parameters parameters)
{
	this->name = name;
	this->timestamp = timestamp;
	this->priority = priority;
	this->parameters = parameters;
}

Lilliputian::Event::Event(std::string name, uint64_t timestamp, Priority priority)
{
	this->name = name;
	this->timestamp = timestamp;
	this->priority = priority;
}

Lilliputian::Event::Event(std::string name, uint64_t timestamp, Parameters parameters)
{
	this->name = name;
	this->timestamp = timestamp;
	this->parameters = parameters;
}

Lilliputian::Event::Event(std::string name, uint64_t timestamp)
{
	this->name = name;
	this->timestamp = timestamp;
	this->priority = Priority::MEDIUM;
}

std::string Lilliputian::Event::getName()
{
	return this->name;
}

uint64_t Lilliputian::Event::getTimestamp()
{
	return this->timestamp;
}

Lilliputian::Event::Priority Lilliputian::Event::getPriority()
{
	return this->priority;
}

Lilliputian::Event::Parameters Lilliputian::Event::getParameters()
{
	return this->parameters;
}