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

#pragma once

#include <stdint.h>

namespace Omnific
{
	class EngineState
	{
	public:
		void setInitializing();
		void setRunningApplicationWindowed();
		void setRunningApplicationFullscreen();
		void setRunningApplicationFullscreenDesktop();
		void setRestarting();
		void setShuttingDown();

		bool isInitializing();
		bool isRunningApplicationWindowed();
		bool isRunningApplicationFullscreen();
		bool isRunningApplicationFullscreenDesktop();
		bool isRestarting();
		bool isShuttingDown();

		bool isRunning();

		EngineState();
	private:
		enum EngineStates
		{
			ENGINE_STATE_INITIALIZING,
			ENGINE_STATE_RUNNING_APPLICATION_WINDOWED,
			ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN,
			ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP,
			ENGINE_STATE_RESTARTING,
			ENGINE_STATE_SHUTTING_DOWN,
		};

		uint8_t value;
	};
}