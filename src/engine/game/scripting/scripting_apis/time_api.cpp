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

#include "time_api.hpp"
#include <utilities/constants.hpp>


void Lilliputian::TimeAPI::initialize(Profiler* profiler)
{
	this->profiler = profiler;
}

void Lilliputian::TimeAPI::setTimeSettings(Configuration::TimeSettings* timeSettings)
{
	this->timeSettings = timeSettings;
}

void Lilliputian::TimeAPI::setMsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	if (msPerComputeUpdate <= (uint32_t)(1.0 / (double) this->timeSettings->targetFPS))
		this->timeSettings->msPerComputeUpdate = msPerComputeUpdate;
}

void Lilliputian::TimeAPI::setTargetFPS(uint32_t targetFPS)
{
	this->timeSettings->targetFPS = targetFPS;
}

uint32_t Lilliputian::TimeAPI::getMsPerComputeUpdate()
{
	return this->timeSettings->msPerComputeUpdate;
}

uint32_t Lilliputian::TimeAPI::getTargetFPS()
{
	return this->timeSettings->targetFPS;
}

uint64_t Lilliputian::TimeAPI::getFrameTimeDelta()
{
	return NS_IN_MS * this->profiler->getFrameTimer().getDelta_ns();
}