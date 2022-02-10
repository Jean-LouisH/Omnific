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

Omnific::EngineState::EngineState()
{
	this->setInitializing();
}

void Omnific::EngineState::setInitializing()
{
	this->value = ENGINE_STATE_INITIALIZING;
}

void Omnific::EngineState::setRunningApplicationWindowed()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

void Omnific::EngineState::setRunningApplicationFullscreen()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

void Omnific::EngineState::setRunningApplicationFullscreenDesktop()
{
	this->value = ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

void Omnific::EngineState::setRestarting()
{
	this->value = ENGINE_STATE_RESTARTING;
}

void Omnific::EngineState::setShuttingDown()
{
	this->value = ENGINE_STATE_SHUTTING_DOWN;
}

bool Omnific::EngineState::isInitializing()
{
	return this->value == ENGINE_STATE_INITIALIZING;
}

bool Omnific::EngineState::isRunningApplicationWindowed()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_WINDOWED;
}

bool Omnific::EngineState::isRunningApplicationFullscreen()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN;
}

bool Omnific::EngineState::isRunningApplicationFullscreenDesktop()
{
	return this->value == ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP;
}

bool Omnific::EngineState::isRestarting()
{
	return this->value == ENGINE_STATE_RESTARTING;
}

bool Omnific::EngineState::isShuttingDown()
{
	return this->value == ENGINE_STATE_SHUTTING_DOWN;
}

bool Omnific::EngineState::isRunning()
{
	return (this->isRunningApplicationFullscreen() ||
		this->isRunningApplicationFullscreenDesktop() ||
		this->isRunningApplicationWindowed());
}