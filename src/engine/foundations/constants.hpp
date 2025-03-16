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

#define DEFAULT_APP_DATA_PATH_NAME "app_data"
#define DEFAULT_APP_DATA_PATH "app_data/"
#define DEFAULT_DEBUG_STUDIO_DATA_PATH "../../../../../app_data/studio_data/"
#define DEFAULT_DEBUG_DEMO_DATA_PATH "../../../../../app_data/demo_data/"

#ifdef _DEBUG
	#define DEBUG_CONSOLE_ENABLED
#endif

#define DEFAULT_TARGET_FPS 60.0
#define DEFAULT_TARGET_INPUT_FPS DEFAULT_TARGET_FPS
#define DEFAULT_TARGET_UPDATE_FPS DEFAULT_TARGET_INPUT_FPS
#define DEFAULT_TARGET_COMPUTE_FPS (DEFAULT_TARGET_UPDATE_FPS * 2.0)
#define DEFAULT_TARGET_OUTPUT_FPS DEFAULT_TARGET_FPS 

#define DEFAULT_FIXED_FRAME_TIME 8

#define BOOT_FILE_NAME "boot.yml"

#define MAIN_THREAD_CLOCK_NAME "main_thread_clock"
#define ENGINE_LOOP_THREAD_CLOCK_NAME "engine_loop_thread_clock"
#define LAG_CLOCK_NAME "lag_clock"
#define TOTAL_LOOP_FRAME_TIME_CLOCK_NAME "total_loop_frame_time_clock"
#define INPUT_FRAME_SKIP_CLOCK_NAME "input_frame_skip_clock"
#define UPDATE_FRAME_SKIP_CLOCK_NAME "update_frame_skip_clock"
#define OUTPUT_FRAME_SKIP_CLOCK_NAME "output_frame_skip_clock"
