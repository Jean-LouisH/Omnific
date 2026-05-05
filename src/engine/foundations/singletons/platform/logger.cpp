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

#include "logger.hpp"
#include <SDL3/SDL_oldnames.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_time.h>


void Omnific::Logger::filter_priority(Priority log_priority)
{
	SDL_SetLogPriorities((SDL_LogPriority)log_priority);
}

void Omnific::Logger::filter_priority_for_category(Category log_category, Priority log_priority)
{
	SDL_SetLogPriority((int)log_category, (SDL_LogPriority)log_priority);
}

void Omnific::Logger::write(std::string message)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_Log(time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_trace(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogTrace((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_verbose(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogVerbose((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_debug(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogDebug((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_info(std::string message)
{
	this->write(message);
}

void Omnific::Logger::write_warning(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogWarn((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_error(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogError((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_critical(std::string message, Category log_category)
{
	std::string time_stamped_message = this->time_stamp(message);
	SDL_LogCritical((int)log_category, time_stamped_message.c_str());
	this->logs.push_back(time_stamped_message);
}

void Omnific::Logger::write_to_file(std::string message)
{
	std::string time_stamped_message = this->time_stamp(message);

	this->logs.push_back(time_stamped_message);
}

std::string Omnific::Logger::get_last_message()
{
	return this->logs.at(this->logs.size() - 1);
}

std::vector<std::string> Omnific::Logger::get_logs()
{
	return this->logs;
}

std::string Omnific::Logger::time_stamp(std::string message)
{
	SDL_Time sdl_time;
	SDL_DateTime sdl_data_time;
	SDL_GetCurrentTime(&sdl_time);
	SDL_TimeToDateTime(sdl_time, &sdl_data_time, true);
	return "[" + std::to_string(sdl_data_time.year) + "/" +
			std::to_string(sdl_data_time.month) + "/" + 
			std::to_string(sdl_data_time.day) + ", " + 
			std::to_string(sdl_data_time.hour) + ":" + 
			std::to_string(sdl_data_time.minute) + ":" +
			std::to_string(sdl_data_time.second) +  "]: " + message;
}