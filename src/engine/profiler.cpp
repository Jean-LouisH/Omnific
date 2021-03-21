//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "profiler.hpp"
#include "utilities/constants.hpp"

Lilliputian::HiResTimer& Lilliputian::Profiler::process()
{
	return this->_process;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::frame()
{
	return this->_frame;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::input()
{
	return this->_input;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::update()
{
	return this->_update;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::output()
{
	return this->_output;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::run()
{
	return this->_run;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::benchmark()
{
	return this->_FPS;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::debug()
{
	return this->_debug;
}

void Lilliputian::Profiler::incrementFrameCount()
{
	this->frameCount++;
}

void Lilliputian::Profiler::incrementLagCount(uint64_t deltaTime_ms)
{
	this->lag_ms += deltaTime_ms;
}

void Lilliputian::Profiler::decrementLagCount(uint64_t deltaTime_ms)
{
	this->lag_ms -= deltaTime_ms;
}

uint64_t Lilliputian::Profiler::getLag_ms()
{
	return this->lag_ms;
}

uint16_t Lilliputian::Profiler::getFPS()
{
	return (1.0 / (this->frame().getDelta_ns() / NS_IN_S));
}