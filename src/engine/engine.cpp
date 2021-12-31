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

#include "engine.hpp"

Esi::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->state = std::unique_ptr<EngineState>(new EngineState());
	this->argc = argc;
	this->argv = argv;
}

void Esi::Engine::run()
{
	do
	{
		if (this->initialize())
		{
			this->application->initialize();
			Configuration configuration = this->application->getConfiguration();

			if (configuration.isLoaded)
			{
				std::string dataDirectory = "data/";
#ifdef _DEBUG
				dataDirectory = DEBUG_DATA_FILEPATH;
#endif
				OS::addGameControllerMappings(dataDirectory + "gamecontrollerdb.txt");
				Window& window = OS::getWindow();
				window.resize(configuration.windowSettings.width, configuration.windowSettings.height);
				window.changeTitle(configuration.metadata.title.c_str());
				this->state->setRunningApplicationWindowed();
			}
			else
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"Could not load game data",
					"The game data is either missing or corrupted. Reinstall and try again",
					NULL
				);
				this->state->setShuttingDown();
			}

		}

		Profiler& profiler = OS::getProfiler();

		while (this->state->isRunning())
		{
			profiler.getFrameTimer().setStart();
			profiler.getProcessTimer().setStart();
			this->input();
			this->update();
			this->output();
			this->benchmark();
			profiler.getProcessTimer().setEnd();
			this->sleep();
			profiler.getFrameTimer().setEnd();
			profiler.incrementFrameCount();
		}

		this->application->deinitialize();
		this->shutdown();
	} while (this->state->isRestarting());
}

bool Esi::Engine::initialize()
{
	bool isInitializedOK = false;

	if (!this->state->isRestarting())
		this->state->setInitializing();

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
		this->state->setShuttingDown();
	}
	else
	{
		OS::initialize("", 640, 480, false, this->argv[0]);

		Platform& platform = OS::getPlatform();
		Logger& logger = OS::getLogger();

		logger.write("Esi initializing...");
		logger.write("Retrieved Logical Core Count: " + std::to_string(platform.getLogicalCoreCount()));
		logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.getL1CacheLineSize_B()) + " B");
		logger.write("Retrieved OS Name: " + platform.getOSName());
		logger.write("Retrieved System RAM: " + std::to_string(platform.getSystemRAM_MB()) + " MB");

		Profiler& profiler = OS::getProfiler();
		profiler.getBenchmarkTimer().setStart();

		this->application = std::unique_ptr<Application>(new Application());

		if (this->state->isInitializing())
		{
			this->aiSystem = std::unique_ptr<AISystem>(new AISystem());
			this->animationSystem = std::unique_ptr<AnimationSystem>(new AnimationSystem());
			this->audioSystem = std::unique_ptr<AudioSystem>(new AudioSystem());
			this->hapticSystem = std::unique_ptr<HapticSystem>(new HapticSystem(&OS::getHid()));
			this->physicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
			this->renderingSystem = std::unique_ptr<RenderingSystem>(new RenderingSystem(OS::getWindow()));
			this->uiSystem = std::unique_ptr<UISystem>(new UISystem(&OS::getHid()));
		}

		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Esi::Engine::input()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getInputTimer().setStart();
	HumanInterfaceDevices& hid = OS::getHid();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state->setShuttingDown();
	if (hid.hasRequestedRestart())
		this->state->setRestarting();

	profiler.getInputTimer().setEnd();
}

void Esi::Engine::update()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getUpdateTimer().setStart();
	Scene& activeScene = this->application->getActiveScene();
	const uint32_t msPerComputeUpdate = this->application->getConfiguration().timeSettings.msPerComputeUpdate;

	this->physicsSystem->setMsPerComputeUpdate(msPerComputeUpdate);
	this->application->executeOnInputMethods();
	this->application->executeOnStartMethods();
	this->application->executeOnFrameMethods();
	this->uiSystem->process(activeScene);
	this->aiSystem->process(activeScene);

	while (profiler.getLag_ms() >= msPerComputeUpdate)
	{
		this->application->executeOnComputeMethods();
		this->animationSystem->process(activeScene);
		this->physicsSystem->process(activeScene);
		profiler.decrementLagCount(msPerComputeUpdate);
	}

	this->application->executeOnOutputMethods();
	this->application->executeOnFinishMethods();
	profiler.incrementLagCount(profiler.getFrameTimer().getDelta_ns() / NS_IN_MS);
	profiler.getUpdateTimer().setEnd();
}

void Esi::Engine::output()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getOutputTimer().setStart();

	this->renderingSystem->process(this->application->getActiveScene());
	this->audioSystem->process(this->application->getActiveScene());
	this->hapticSystem->process(this->application->getActiveScene());

	profiler.getOutputTimer().setEnd();
}

void Esi::Engine::benchmark()
{
	Profiler& profiler = OS::getProfiler();
#ifdef _DEBUG
	uint32_t FPSUpdateSeconds = 1;

	if (profiler.getBenchmarkTimer().getDelta_ns() / NS_IN_MS >= (FPSUpdateSeconds * MS_IN_S))
	{
		profiler.getBenchmarkTimer().setStart();
		std::string FPSString = std::to_string(profiler.getFPS());
		std::string frameUtilizationString =
			std::to_string((int)(((double)profiler.getProcessTimer().getDelta_ns() / (double)profiler.getFrameTimer().getDelta_ns()) * 100));
		OS::getWindow().changeTitle((this->application->getConfiguration().metadata.title + " (DEBUG) ->" +
			" FPS: " + FPSString).c_str()
		);
	}
#endif
	profiler.getBenchmarkTimer().setEnd();
}

void Esi::Engine::sleep()
{
	Profiler& profiler = OS::getProfiler();
	float targetFrameTime_ms = 1000.0 / this->application->getConfiguration().timeSettings.targetFPS;
	float processTime_ms = profiler.getProcessTimer().getDelta_ns() / NS_IN_MS;
	OS::getWindow().sleep(targetFrameTime_ms - processTime_ms);
}

void Esi::Engine::shutdown()
{
	this->application.reset();
}