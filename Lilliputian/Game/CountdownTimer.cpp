#include "CountdownTimer.hpp"

void Lilliputian::CountdownTimer::start(double time)
{
	this->startTime = time;
	this->currentTime = startTime;
	this->isActive = true;
}

void Lilliputian::CountdownTimer::update(double deltaTime)
{
	if (this->isActive && this->currentTime > 0.0)
		this->currentTime -= deltaTime;
}

void Lilliputian::CountdownTimer::stop()
{
	this->isActive = false;
}

bool Lilliputian::CountdownTimer::isFinished()
{
	return this->currentTime <= 0.0;
}