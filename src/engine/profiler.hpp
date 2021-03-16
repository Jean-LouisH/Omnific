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
		HiResTimer& input();
		HiResTimer& update();
		HiResTimer& output();
		HiResTimer& run();
		HiResTimer& benchmark();
		HiResTimer& debug();
		void incrementFrameCount();
		void incrementLagCount(uint64_t deltaTime_ms);
		void decrementLagCount(uint64_t deltaTime_ms);
		uint64_t getLag_ms();
		uint16_t getFPS();
	private:
		HiResTimer _process;
		HiResTimer _frame;
		HiResTimer _input;
		HiResTimer _update;
		HiResTimer _output;
		HiResTimer _run;
		HiResTimer _FPS;
		HiResTimer _debug;
		uint64_t lag_ms = 0;
		uint64_t frameCount = 0;
	};
}