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

#include <stdint.h>
#include "utilities/hi_res_timer.hpp"

namespace Omnific
{
	class Profiler
	{
	public:
		HiResTimer& getProcessTimer();
		HiResTimer& getFrameTimer();
		HiResTimer& getInputTimer();
		HiResTimer& getUpdateTimer();
		HiResTimer& getOutputTimer();
		HiResTimer& getBenchmarkTimer();
		HiResTimer& getDebugTimer();
		void incrementFrameCount();
		void incrementLagCount(uint64_t deltaTime_ms);
		void decrementLagCount(uint64_t deltaTime_ms);
		uint64_t getLag_ms();
		uint16_t getFPS();
	private:
		HiResTimer process;
		HiResTimer frame;
		HiResTimer input;
		HiResTimer update;
		HiResTimer output;
		HiResTimer FPS;
		HiResTimer debug;
		uint64_t lag_ms = 0;
		uint64_t frameCount = 0;
	};
}