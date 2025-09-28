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

#include "clock.hpp"
#include "constants.hpp"
#include <string>
#include "SDL.h"

Omnific::Clock::Clock()
{
	this->clock_name = "unnamed";
}

Omnific::Clock::Clock(std::string clock_name)
{
	this->clock_name = clock_name;
}

std::string Omnific::Clock::get_name()
{
	return this->clock_name;
}

uint64_t Omnific::Clock::get_delta()
{
	return this->delta;
}

float Omnific::Clock::get_delta_in_seconds()
{
	return this->delta / MS_IN_S;
}

void Omnific::Clock::set_start()
{
	this->start = SDL_GetTicks();
}

void Omnific::Clock::set_end()
{
	this->finish = SDL_GetTicks();
	this->delta = finish - start;
}
