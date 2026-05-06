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

#include <vector>
#include <string>
#include <engine_api.hpp>
#include <SDL3/SDL_log.h>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Logger
	{
	public:
		enum class Category
		{
			APPLICATION = SDL_LOG_CATEGORY_APPLICATION,
			ERROR = SDL_LOG_CATEGORY_ERROR,
			ASSERT = SDL_LOG_CATEGORY_ASSERT,
			SYSTEM = SDL_LOG_CATEGORY_SYSTEM,
			AUDIO =SDL_LOG_CATEGORY_AUDIO,
			VIDEO = SDL_LOG_CATEGORY_VIDEO,
			RENDER = SDL_LOG_CATEGORY_RENDER,
			INPUT = SDL_LOG_CATEGORY_INPUT,
			TEST = SDL_LOG_CATEGORY_TEST,
			GPU = SDL_LOG_CATEGORY_GPU
		};

		enum class Priority
		{
			TRACE = SDL_LOG_PRIORITY_TRACE,
			VERBOSE = SDL_LOG_PRIORITY_VERBOSE,
			DEBUG = SDL_LOG_PRIORITY_DEBUG,
			INFO = SDL_LOG_PRIORITY_INFO,
			WARN = SDL_LOG_PRIORITY_WARN,
			ERROR = SDL_LOG_PRIORITY_ERROR,
			CRITICAL = SDL_LOG_PRIORITY_CRITICAL
		};

		void filter_priority(Priority log_priority);
		void filter_priority_for_category(Category log_category, Priority log_priority);
		void write(std::string message);
		void write_trace(std::string, Category log_category);
		void write_verbose(std::string, Category log_category);
		void write_debug(std::string, Category log_category);
		void write_info(std::string);
		void write_warning(std::string, Category log_category);
		void write_error(std::string, Category log_category);
		void write_critical(std::string, Category log_category);
		void write_to_file(std::string message);
		std::string get_last_message();
		std::vector<std::string> get_logs();
	private:
		std::string time_stamp(std::string message);
		std::vector<std::string> logs;
	};
}