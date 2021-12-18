// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "engine_state.hpp"

Esi::EngineState::EngineState()
{
	this->setInitializing();
}

void Esi::EngineState::setInitializing()
{
	this->value = ENGINE_STATE_INITIALIZING;
}

void Esi::EngineState::setRunningApplicationWindowed()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

void Esi::EngineState::setRunningApplicationFullscreen()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

void Esi::EngineState::setRunningApplicationFullscreenDesktop()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

void Esi::EngineState::setRestarting()
{
	this->value = ENGINE_STATE_RESTARTING;
}

void Esi::EngineState::setShuttingDown()
{
	this->value = ENGINE_STATE_SHUTTING_DOWN;
}

bool Esi::EngineState::isInitializing()
{
	return this->value == ENGINE_STATE_INITIALIZING;
}

bool Esi::EngineState::isRunningApplicationWindowed()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

bool Esi::EngineState::isRunningApplicationFullscreen()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

bool Esi::EngineState::isRunningApplicationFullscreenDesktop()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

bool Esi::EngineState::isRestarting()
{
	return this->value == ENGINE_STATE_RESTARTING;
}

bool Esi::EngineState::isShuttingDown()
{
	return this->value == ENGINE_STATE_SHUTTING_DOWN;
}

bool Esi::EngineState::isRunning()
{
	return (this->isRunningApplicationFullscreen() ||
		this->isRunningApplicationFullscreenDesktop() ||
		this->isRunningApplicationWindowed());
}