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

#define DUMMY_ENTITY 0 

#define DEBUG_EDITOR_DATA_FILEPATH "../../../../../data/editor/"
#define DEBUG_DEMO_DATA_FILEPATH "../../../../../data/demos/"
#define DEBUG_DEBUG_DATA_FILEPATH "../../../../../data/esi_debug/"
#define DEBUG_DATA_FILEPATH DEBUG_DEBUG_DATA_FILEPATH

#define DEBUG_DEMO_MODE false

#if (DEBUG_DEMO_MODE)
	#define DEBUG_DATA_FILEPATH DEBUG_DEMO_DATA_FILEPATH
#endif

#if _DEBUG
	#define DEBUG_CONSOLE_ENABLED
#endif