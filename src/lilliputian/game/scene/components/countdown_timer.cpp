#include "countdown_timer.hpp"

void Lilliputian::CountdownTimer::start(float time)
{
	this->startTime = time;
	this->currentTime = startTime;
	this->isActive = true;
}

void Lilliputian::CountdownTimer::update(float deltaTime)
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