//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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