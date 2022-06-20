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
#include <thread>

Omnia::OS* Omnia::OS::instance = nullptr;

bool Omnia::OS::initialize(
	std::string title, 
	uint16_t width, 
	uint16_t height, 
	bool isFullscreen, 
	std::string executableFilepath,
	std::string renderingContext)
{
	OS* newInstance = getInstance();
	bool isSuccessful = false;

	isSuccessful = !(bool)SDL_Init(SDL_INIT_EVERYTHING);

	if (isSuccessful)
	{
		newInstance->dllAccess = std::unique_ptr<DynamicLinkLibraryAccess>(new DynamicLinkLibraryAccess());
		newInstance->logger = std::unique_ptr<Logger>(new Logger());
		newInstance->window = std::unique_ptr<Window>(new Window(title, width, height, isFullscreen, renderingContext));
		newInstance->input = std::unique_ptr<Input>(new Input());
		newInstance->fileAccess = std::unique_ptr<FileAccess>(new FileAccess(executableFilepath));
		newInstance->networkAccess = std::unique_ptr<NetworkAccess>(new NetworkAccess());
		newInstance->profiler = std::unique_ptr<Profiler>(new Profiler());
		newInstance->platform = std::unique_ptr<Platform>(new Platform());
		newInstance->threadPool = std::unique_ptr<ThreadPool>(new ThreadPool());
		newInstance->runTimer = std::unique_ptr<HiResTimer>(new HiResTimer());
		newInstance->runTimer->setStart();
	}
	else
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
	}

	return isSuccessful;
}

void Omnia::OS::yieldThisThread()
{
	std::this_thread::yield();
}

void Omnia::OS::sleepThisThreadFor(int duration)
{
	if (duration > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

void Omnia::OS::showErrorBox(std::string title, std::string message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
		title.c_str(),
		message.c_str(),
		NULL
	);
}

void Omnia::OS::deinitialize()
{
	SDL_Quit();
	getInstance()->getThreadPool().deinitialize();
	delete getInstance();
	instance = nullptr;
}

Omnia::DynamicLinkLibraryAccess& Omnia::OS::getDLLAccess()
{
	return *getInstance()->dllAccess;
}

Omnia::Window& Omnia::OS::getWindow()
{
	return *getInstance()->window;
}

Omnia::Input& Omnia::OS::getInput()
{
	return *getInstance()->input;
}

Omnia::Logger& Omnia::OS::getLogger()
{
	return *getInstance()->logger;
}

Omnia::FileAccess& Omnia::OS::getFileAccess()
{
	return *getInstance()->fileAccess;
}

Omnia::NetworkAccess& Omnia::OS::getNetworkAccess()
{
	return *getInstance()->networkAccess;
}

Omnia::Profiler& Omnia::OS::getProfiler()
{
	return *getInstance()->profiler;
}

Omnia::Platform& Omnia::OS::getPlatform()
{
	return *getInstance()->platform;
}

Omnia::ThreadPool& Omnia::OS::getThreadPool()
{
	return *getInstance()->threadPool;
}

Omnia::HiResTimer& Omnia::OS::getRunTimer()
{
	return *getInstance()->runTimer;
}

void Omnia::OS::addGameControllerMappings(std::string mappingFilepath)
{
	SDL_GameControllerAddMappingsFromFile(mappingFilepath.c_str());
}

Omnia::OS* Omnia::OS::getInstance()
{
	if (instance == nullptr)
		instance = new OS();
	return instance;
}