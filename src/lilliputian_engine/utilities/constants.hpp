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

#define DEBUG_EDITOR_DATA_FILEPATH "../../../../../data/editor/"
#define DEBUG_DEMO_DATA_FILEPATH "../../../../../data/demos/"
#define DEBUG_DATA_FILEPATH DEBUG_EDITOR_DATA_FILEPATH
#define DEBUG_DEMO_MODE false

#if (DEBUG_DEMO_MODE)
	#define DEBUG_DATA_FILEPATH DEBUG_DEMO_DATA_FILEPATH
#endif
}
