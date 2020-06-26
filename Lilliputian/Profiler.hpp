#pragma once

#include <stdint.h>
#include "Utilities/HiResTimer.hpp"

namespace Lilliputian
{
	class Profiler
	{
	public:
		HiResTimer process;
		uint64_t lag_ms;
		Profiler() :
			lag_ms(0)
		{}
	};
}