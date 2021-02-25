#include "profiler.hpp"

Lilliputian::HiResTimer& Lilliputian::Profiler::process()
{
	return this->processTimer;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::frame()
{
	return this->frameTimer;
}

void Lilliputian::Profiler::incrementFrameCount()
{
	this->frameCount++;
}

void Lilliputian::Profiler::incrementLagCount(uint64_t deltaTime_ms)
{
	this->lag_ms += deltaTime_ms;
}

void Lilliputian::Profiler::decrementLagCount(uint64_t deltaTime_ms)
{
	this->lag_ms -= deltaTime_ms;
}

uint64_t Lilliputian::Profiler::getLag_ms()
{
	return this->lag_ms;
}