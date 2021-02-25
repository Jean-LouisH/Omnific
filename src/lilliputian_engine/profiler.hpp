#pragma once

#include <stdint.h>
#include "utilities/hi_res_timer.hpp"

namespace Lilliputian
{
	class Profiler
	{
	public:
		HiResTimer& process();
		HiResTimer& frame();
		void incrementFrameCount();
		void incrementLagCount(uint64_t deltaTime_ms);
		void decrementLagCount(uint64_t deltaTime_ms);
		uint64_t getLag_ms();
	private:
		HiResTimer processTimer;
		HiResTimer frameTimer;
		uint64_t lag_ms = 0;
		uint64_t frameCount = 0;
		uint16_t FPS = 0;
	};
}