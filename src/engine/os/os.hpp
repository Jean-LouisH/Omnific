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

#include <SDL.h>
#include <vector>
#include "dynamic_link_library_access.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "profiler.hpp"
#include "file_access.hpp"
#include "network_access.hpp"
#include "platform.hpp"
#include "thread_pool.hpp"
#include <string>

namespace Omnia
{
	/* Singleton for objects related to Operating System utilities. */
	class OS
	{
	public:
		static bool initialize(
			std::string title, 
			uint16_t width, 
			uint16_t height, 
			bool isFullscreen, 
			std::string executableFilepath,
			std::string renderingContext);

		static void yieldThisThread();
		/* Duration in milliseconds */
		static void sleepThisThreadFor(int duration);

		static void showErrorBox(std::string title, std::string message);

		static void deinitialize();

		static DynamicLinkLibraryAccess& getDLLAccess();
		static Window& getWindow();
		static Input& getInput();
		static Logger& getLogger();
		static FileAccess& getFileAccess();
		static NetworkAccess& getNetworkAccess();
		static Profiler& getProfiler();
		static Platform& getPlatform();
		static ThreadPool& getThreadPool();

		static HiResTimer& getRunTimer();
		static void addGameControllerMappings(std::string mappingFilepath);
	private:
		static OS* instance;

		std::unique_ptr<DynamicLinkLibraryAccess> dllAccess;
		std::unique_ptr<HiResTimer> runTimer;
		std::unique_ptr<Window> window;
		std::unique_ptr<Input> input;
		std::unique_ptr<Logger> logger;
		std::unique_ptr<FileAccess> fileAccess;
		std::unique_ptr<NetworkAccess> networkAccess;
		std::unique_ptr<Profiler> profiler;
		std::unique_ptr<Platform> platform;
		std::unique_ptr<ThreadPool> threadPool;

		static OS* getInstance();
	};
}