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

#include "profiler.hpp"
#include "core/utilities/constants.hpp"
#include <core/singletons/os/os.hpp>

uint64_t Omnia::HiResTimer::getDeltaInNanoseconds()
{
	return this->delta;
}

uint64_t Omnia::HiResTimer::getDelta()
{
	return this->delta / NS_IN_MS;
}

float Omnia::HiResTimer::getDeltaInSeconds()
{
	return this->delta / NS_IN_S;
}

void Omnia::HiResTimer::setStart()
{
	this->start = std::chrono::high_resolution_clock::now();
}

void Omnia::HiResTimer::setEnd()
{
	this->end = std::chrono::high_resolution_clock::now();
	this->delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

void Omnia::Profiler::addTimer(std::string timerName, bool isRemovable)
{
	this->timers.emplace(timerName, std::shared_ptr<HiResTimer>(new HiResTimer()));
	this->isRemovableMap.emplace(timerName, isRemovable);
	OS::getLogger().write("Added HiResTimer to Profiler: \"" + timerName + "\"");
}

void Omnia::Profiler::removeTimer(std::string timerName)
{
	if (this->isRemovableMap.count(timerName))
	{
		if (this->isRemovableMap.at(timerName))
		{
			this->timers.erase(timerName);
			this->isRemovableMap.erase(timerName);
		}
	}
}

std::shared_ptr<Omnia::HiResTimer> Omnia::Profiler::getTimer(std::string timerName)
{
	std::shared_ptr<HiResTimer> timer;

	if (this->timers.count(timerName) > 0)
		timer = this->timers.at(timerName);

	return timer;
}

void Omnia::Profiler::incrementFrameCount()
{
	this->frameCount++;
}

void Omnia::Profiler::incrementLagCount(uint64_t deltaTime_ms)
{
	this->lag += deltaTime_ms;
}

void Omnia::Profiler::decrementLagCount(uint64_t deltaTime_ms)
{
	this->lag -= deltaTime_ms;
}

uint64_t Omnia::Profiler::getLagCount()
{
	return this->lag;
}

uint16_t Omnia::Profiler::getFPS()
{
	return (1.0 / (this->getTimer("frame")->getDeltaInNanoseconds() / NS_IN_S));
}