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

#pragma once

#include "event.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API EventBus
	{
	public:
		EventBus()
		{
			/* Reserve some event space to prevent initial frame lag */
			events.reserve(32);
		}

		void publish(
			std::string name,
			std::unordered_map<std::string, double> numbers,
			std::unordered_map<std::string, std::string> strings);

		void publish(
			std::string name,
			std::unordered_map<std::string, double> numbers);

		void publish(
			std::string name,
			std::unordered_map<std::string, std::string> strings);

		void publish(
			std::string name);

		void clear();

		std::vector<Event> query(std::string name);
	private:
		std::unordered_map<std::string, std::vector<Event>> events;

		void publishWithParameters(std::string name, Event::Parameters parameters);
	};
}