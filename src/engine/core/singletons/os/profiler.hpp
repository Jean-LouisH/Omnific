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
#include "core/utilities/hi_res_timer.hpp"
#include <unordered_map>
#include <memory>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API Profiler
	{
	public:
		void addTimer(std::string timerName, bool isRemovable = true);
		void removeTimer(std::string timerName);
		std::shared_ptr<HiResTimer> getTimer(std::string timerName);

		void incrementFrameCount();
		void incrementLagCount(uint64_t deltaTime);
		void decrementLagCount(uint64_t deltaTime);
		/* In milliseconds */
		uint64_t getLagCount();
		uint16_t getFPS();
	private:
		std::unordered_map<std::string, std::shared_ptr<HiResTimer>> timers;
		std::unordered_map<std::string, bool> isRemovableMap;
		/* In milliseconds */
		uint64_t lag = 0;
		uint64_t frameCount = 0;
	};
}