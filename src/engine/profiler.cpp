#include "profiler.hpp"
#include "utilities/constants.hpp"

Lilliputian::HiResTimer& Lilliputian::Profiler::process()
{
	return this->_process;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::frame()
{
	return this->_frame;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::input()
{
	return this->_input;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::update()
{
	return this->_update;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::output()
{
	return this->_output;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::run()
{
	return this->_run;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::benchmark()
{
	return this->_FPS;
}

Lilliputian::HiResTimer& Lilliputian::Profiler::debug()
{
	return this->_debug;
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

uint16_t Lilliputian::Profiler::getFPS()
{
	return (1.0 / (this->frame().getDelta_ns() / NS_IN_S));
}