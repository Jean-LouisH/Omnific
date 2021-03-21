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

#pragma once

namespace Lilliputian
{
#define MS_PER_COMPUTE_UPDATE 8

#define TERA 1000000000000.0
#define GIGA 1000000000.0
#define MEGA 1000000.0
#define KILO 1000.0
#define MILLI 0.001
#define MICRO 0.000001
#define NANO 0.000000001
#define PICO 0.000000000001

#define NS_IN_S (1.0 / NANO)
#define NS_IN_MS (MILLI / NANO)
#define MS_IN_S (1.0 / MILLI)
#define S_PER_COMPUTE_UPDATE ((double)MS_PER_COMPUTE_UPDATE / (double)MS_IN_S)
#define COMPUTE_UPDATES_PER_S (1.0 / (double)S_PER_COMPUTE_UPDATE)

#define EARTH_GRAVITY 9.80665
#define GRAVITATIONAL_CONSTANT 6.67408e-11
#define EARTH_MASS 5.9722e24

#define NO_ENTITY 0 
#define NO_TRANSFORM 0

#define DEBUG_EDITOR_DATA_FILEPATH "../../../../../data/editor/"
#define DEBUG_DEMO_DATA_FILEPATH "../../../../../data/demos/"
#define DEBUG_DATA_FILEPATH DEBUG_EDITOR_DATA_FILEPATH
#define DEBUG_DEMO_MODE false

#if (DEBUG_DEMO_MODE)
	#define DEBUG_DATA_FILEPATH DEBUG_DEMO_DATA_FILEPATH
#endif
}
