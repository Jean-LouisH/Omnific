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

Omnia::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->state = std::unique_ptr<EngineState>(new EngineState());
	this->argc = argc;
	this->argv = argv;
}

void Omnia::Engine::run()
{
	do
	{
		/* On successful initialization */
		if (this->initialize())
		{
			this->application->initialize();

			/* Configuration data is loaded and applied 
			   to the Window at boot. */
			std::shared_ptr<Configuration> configuration = this->application->getConfiguration();

			if (configuration->isLoaded)
			{
				std::string dataDirectory = "data/";
#ifdef _DEBUG
				dataDirectory = DEBUG_DATA_FILEPATH;
#endif
				OS::addGameControllerMappings(dataDirectory + "gamecontrollerdb.txt");
				Window& window = OS::getWindow();
				window.resize(configuration->windowSettings.width, configuration->windowSettings.height);
				window.changeTitle(configuration->metadata.title.c_str());
				this->state->setRunningApplicationWindowed();
				OS::getLogger().write("Loaded application project \"" + configuration->metadata.title + "\" at: " + dataDirectory);
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

		/* Main engine loop */
		/* Single-threaded for now. */
		while (this->state->isRunning())
		{
			profiler.getTimer("frame")->setStart();
			profiler.getTimer("process")->setStart();
			this->input();
			this->update();
			this->output();
			this->benchmark();
			profiler.getTimer("process")->setEnd();
			this->sleep();
			profiler.getTimer("frame")->setEnd();
			profiler.incrementFrameCount();
		}

		this->application->deinitialize();
		this->shutdown();
	} while (this->state->isRestarting());
}

bool Omnia::Engine::initialize()
{
	bool isInitializedOK = false;

	this->aiSystem = std::unique_ptr<AISystem>(new AISystem());
	this->animationSystem = std::unique_ptr<AnimationSystem>(new AnimationSystem());
	this->audioSystem = std::unique_ptr<AudioSystem>(new AudioSystem());
	this->hapticSystem = std::unique_ptr<HapticSystem>(new HapticSystem());
	this->physicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
	this->renderingSystem = std::unique_ptr<RenderingSystem>(new RenderingSystem());
	this->uiSystem = std::unique_ptr<UISystem>(new UISystem());

	if (!this->state->isRestarting())
		this->state->setInitializing();

	/* Hardware abstraction layer initialization. */
	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
		this->state->setShuttingDown();
	}
	else
	{
		OS::initialize("", 640, 480, false, this->argv[0], this->renderingSystem->getRenderingContextName());

		Profiler& profiler = OS::getProfiler();

		/* These timers persist throughout Engine runtime and 
		   keep track of elapsed times in nanoseconds. */
		profiler.addTimer("process");
		profiler.addTimer("frame");
		profiler.addTimer("input");
		profiler.addTimer("update");
		profiler.addTimer("output");
		profiler.addTimer("benchmark");
		profiler.addTimer("debug");

		profiler.getTimer("benchmark")->setStart();

		/* System initializations are delayed until 
		   the hardware abstraction layer is loaded. */
		this->aiSystem->initialize();
		this->animationSystem->initialize();
		this->audioSystem->initialize();
		this->hapticSystem->initialize();
		this->physicsSystem->initialize();
		this->renderingSystem->initialize();
		this->uiSystem->initialize();

		Platform& platform = OS::getPlatform();
		Logger& logger = OS::getLogger();

		logger.write("Retrieved Logical Core Count: " + std::to_string(platform.getLogicalCoreCount()));
		logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.getL1CacheLineSize()) + " B");
		logger.write("Retrieved OS Name: " + platform.getOSName());
		logger.write("Retrieved System RAM: " + std::to_string(platform.getSystemRAM()) + " MB");

		this->application = std::unique_ptr<Application>(new Application());
		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Omnia::Engine::input()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getTimer("input")->setStart();
	Input& hid = OS::getInput();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state->setShuttingDown();
	if (hid.hasRequestedRestart())
		this->state->setRestarting();

	profiler.getTimer("input")->setEnd();
}

void Omnia::Engine::update()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getTimer("update")->setStart();
	std::shared_ptr<Scene> activeScene = this->application->getActiveScene();
	const uint32_t msPerComputeUpdate = this->application->getConfiguration()->timeSettings.msPerComputeUpdate;

	this->animationSystem->setMsPerComputeUpdate(msPerComputeUpdate);
	this->physicsSystem->setMsPerComputeUpdate(msPerComputeUpdate);
	this->application->executeOnInputMethods();
	this->application->executeOnStartMethods();
	this->application->executeOnLogicFrameMethods();
	this->uiSystem->process(activeScene);
	this->aiSystem->process(activeScene);

	/* This calls the compute based Systems repeatedly until the accumulated
	   lag milliseconds are depleted. This ensures compute operations
	   are accurate to real-time, even when frames drop. */

	while (profiler.getLagCount() >= msPerComputeUpdate)
	{
		this->application->executeOnComputeFrameMethods();
		this->animationSystem->process(activeScene);
		this->physicsSystem->process(activeScene);
		profiler.decrementLagCount(msPerComputeUpdate);
	}

	this->physicsSystem->onComputeEnd(activeScene);
	this->application->executeOnOutputMethods();
	this->application->executeOnFinishMethods();
	profiler.incrementLagCount(profiler.getTimer("frame")->getDelta());
	profiler.getTimer("update")->setEnd();
}

void Omnia::Engine::output()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getTimer("output")->setStart();

	this->renderingSystem->process(this->application->getActiveScene());
	this->audioSystem->process(this->application->getActiveScene());
	this->hapticSystem->process(this->application->getActiveScene());

	profiler.getTimer("output")->setEnd();
}

void Omnia::Engine::benchmark()
{
	Profiler& profiler = OS::getProfiler();
#ifdef _DEBUG
	uint32_t FPSUpdateSeconds = 1;

	if (profiler.getTimer("benchmark")->getDeltaInNanoseconds() / NS_IN_MS >= (FPSUpdateSeconds * MS_IN_S))
	{
		profiler.getTimer("benchmark")->setStart();
		std::string FPSString = std::to_string(profiler.getFPS());
		std::string frameUtilizationString =
			std::to_string((int)(((double)profiler.getTimer("process")->getDeltaInNanoseconds() / (double)profiler.getTimer("frame")->getDeltaInNanoseconds()) * 100));
		OS::getWindow().changeTitle((this->application->getConfiguration()->metadata.title + " (DEBUG) ->" +
			" FPS: " + FPSString).c_str()
		);
	}
#endif
	profiler.getTimer("benchmark")->setEnd();
}

void Omnia::Engine::sleep()
{
	Profiler& profiler = OS::getProfiler();
	float targetFrameTime = 1000.0 / this->application->getConfiguration()->timeSettings.targetFPS;
	float processTime = profiler.getTimer("process")->getDelta();
	OS::getWindow().sleep(targetFrameTime - processTime);
}

void Omnia::Engine::shutdown()
{
	this->application.reset();
	this->aiSystem.reset();
	this->animationSystem.reset();
	this->audioSystem.reset();
	this->hapticSystem.reset();
	this->physicsSystem.reset();
	this->renderingSystem.reset();
	this->uiSystem.reset();
	OS::deinitialize();
	SDL_Quit();
}