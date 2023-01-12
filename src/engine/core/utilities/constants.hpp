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

#include "orders_of_magnitude.hpp"
#include "time_conversions.hpp"
#include "physics_constants.hpp"

#define DEBUG_EDITOR_DATA_FILEPATH "../../../../../data/editor_data/"
#define DEBUG_DEMO_DATA_FILEPATH "../../../../../data/demo_data/"
#define DEBUG_DEBUG_DATA_FILEPATH "../../../../../data/debug_data/"

#ifdef _DEBUG
	#define DEBUG_CONSOLE_ENABLED
#endif

#define INPUT_TARGET_FPS 60.0
#define UPDATE_TARGET_FPS INPUT_TARGET_FPS
#define COMPUTE_MINIMUM_TARGET_FPS (UPDATE_TARGET_FPS * 2.0)
#define MAXIMUM_MS_PER_COMPUTE_UPDATE (1000.0 / COMPUTE_MINIMUM_TARGET_FPS)

#define MAIN_THREAD_TIMER_NAME "main_thread"
#define UPDATE_THREAD_TIMER_NAME "update_thread"
#define UPDATE_FRAME_TIMER_NAME "update_frame"
#define OUTPUT_THREAD_TIMER_NAME "output_thread"