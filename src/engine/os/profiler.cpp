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

Omnia::HiResTimer& Omnia::Profiler::getProcessTimer()
{
	return this->process;
}

Omnia::HiResTimer& Omnia::Profiler::getFrameTimer()
{
	return this->frame;
}

Omnia::HiResTimer& Omnia::Profiler::getInputTimer()
{
	return this->input;
}

Omnia::HiResTimer& Omnia::Profiler::getUpdateTimer()
{
	return this->update;
}

Omnia::HiResTimer& Omnia::Profiler::getOutputTimer()
{
	return this->output;
}

Omnia::HiResTimer& Omnia::Profiler::getBenchmarkTimer()
{
	return this->FPS;
}

Omnia::HiResTimer& Omnia::Profiler::getDebugTimer()
{
	return this->debug;
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
	return (1.0 / (this->getFrameTimer().getDeltaInNanoseconds() / NS_IN_S));
}