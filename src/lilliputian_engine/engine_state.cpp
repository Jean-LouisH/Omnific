#include "engine_state.hpp"

Lilliputian::EngineState::EngineState()
{
	this->setInitializing();
}

void Lilliputian::EngineState::setInitializing()
{
	this->value = ENGINE_STATE_INITIALIZING;
}

void Lilliputian::EngineState::setRunningApplicationWindowed()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

void Lilliputian::EngineState::setRunningApplicationFullscreen()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

void Lilliputian::EngineState::setRunningApplicationFullscreenDesktop()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

void Lilliputian::EngineState::setRestarting()
{
	this->value = ENGINE_STATE_RESTARTING;
}

void Lilliputian::EngineState::setShuttingDown()
{
	this->value = ENGINE_STATE_SHUTTING_DOWN;
}

bool Lilliputian::EngineState::isInitializing()
{
	return this->value == ENGINE_STATE_INITIALIZING;
}

bool Lilliputian::EngineState::isRunningApplicationWindowed()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

bool Lilliputian::EngineState::isRunningApplicationFullscreen()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

bool Lilliputian::EngineState::isRunningApplicationFullscreenDesktop()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

bool Lilliputian::EngineState::isRestarting()
{
	return this->value == ENGINE_STATE_RESTARTING;
}

bool Lilliputian::EngineState::isShuttingDown()
{
	return this->value == ENGINE_STATE_SHUTTING_DOWN;
}

bool Lilliputian::EngineState::isRunning()
{
	return (this->isRunningApplicationFullscreen() ||
		this->isRunningApplicationFullscreenDesktop() ||
		this->isRunningApplicationWindowed());
}