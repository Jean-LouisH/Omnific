#include "hi_res_timer.hpp"

uint64_t Lilliputian::HiResTimer::getDelta_ns()
{
	return this->delta_ns;
}

void Lilliputian::HiResTimer::setStart()
{
	this->start = std::chrono::high_resolution_clock::now();
}

void Lilliputian::HiResTimer::setEnd()
{
	this->end = std::chrono::high_resolution_clock::now();
	this->delta_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}