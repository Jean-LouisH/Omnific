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

#include "os.hpp"
#include <string>
#include "SDL.h"
#include "utilities/constants.hpp"

Omnific::OS* Omnific::OS::instance = nullptr;

void Omnific::OS::initialize(
	std::string title, 
	uint16_t width, 
	uint16_t height, 
	bool isFullscreen, 
	std::string executableFilepath,
	std::string renderingContext)
{
	OS* newInstance = getInstance();

	newInstance->dllAccess = std::unique_ptr<DynamicLinkLibraryAccess>(new DynamicLinkLibraryAccess());
	newInstance->logger = std::unique_ptr<Logger>(new Logger());
	newInstance->window = std::unique_ptr<Window>(new Window(title, width, height, isFullscreen, renderingContext));
	newInstance->hid = std::unique_ptr<HumanInterfaceDevices>(new HumanInterfaceDevices());
	newInstance->fileAccess = std::unique_ptr<FileAccess>(new FileAccess(executableFilepath));
	newInstance->profiler = std::unique_ptr<Profiler>(new Profiler());
	newInstance->platform = std::unique_ptr<Platform>(new Platform());
	newInstance->threadPool = std::unique_ptr<ThreadPool>(new ThreadPool());
	newInstance->runTimer = std::unique_ptr<HiResTimer>(new HiResTimer());
	newInstance->runTimer->setStart();
}

void Omnific::OS::deinitialize()
{
	delete getInstance();
	instance = nullptr;
}

Omnific::DynamicLinkLibraryAccess& Omnific::OS::getDLLAccess()
{
	return *getInstance()->dllAccess;
}

Omnific::Window& Omnific::OS::getWindow()
{
	return *getInstance()->window;
}

Omnific::HumanInterfaceDevices& Omnific::OS::getHid()
{
	return *getInstance()->hid;
}

Omnific::Logger& Omnific::OS::getLogger()
{
	return *getInstance()->logger;
}

Omnific::FileAccess& Omnific::OS::getFileAccess()
{
	return *getInstance()->fileAccess;
}

Omnific::Profiler& Omnific::OS::getProfiler()
{
	return *getInstance()->profiler;
}

Omnific::Platform& Omnific::OS::getPlatform()
{
	return *getInstance()->platform;
}

Omnific::ThreadPool& Omnific::OS::getThreadPool()
{
	return *getInstance()->threadPool;
}

Omnific::HiResTimer& Omnific::OS::getRunTimer()
{
	return *getInstance()->runTimer;
}

void Omnific::OS::addGameControllerMappings(std::string mappingFilepath)
{
	SDL_GameControllerAddMappingsFromFile(mappingFilepath.c_str());
}

Omnific::OS* Omnific::OS::getInstance()
{
	if (instance == nullptr)
		instance = new OS();
	return instance;
}