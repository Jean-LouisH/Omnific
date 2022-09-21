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
#include "utilities/constants.hpp"
#include <singletons/os/os.hpp>

void Omnia::Profiler::addTimer(std::string timerName)
{
	this->timers.emplace(timerName, std::shared_ptr<HiResTimer>(new HiResTimer()));
	OS::getLogger().write("Added HiResTimer to Profiler: \"" + timerName + "\"");
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