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
#include <thread>

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
				OS::showErrorBox(
					"Could not load game data", 
					"The game data is either missing or corrupted. Reinstall and try again");
				this->state->setShuttingDown();
			}

		}

		Profiler& profiler = OS::getProfiler();

		/* These timers persist throughout Engine runtime and
		   keep track of elapsed times in nanoseconds. */
		profiler.addTimer("main_loop");
		profiler.addTimer("input");
		profiler.addTimer("update");
		profiler.addTimer("rendering");
		profiler.addTimer("audio");
		profiler.addTimer("haptic");

		/* Engine threading uses a hybrid of dedicated threads
		   for deadline sensitive tasks and a thread pool for 
		   general parallelizable tasks. */

		std::vector<std::thread> dedicatedThreads;
		dedicatedThreads.push_back(std::thread(&Engine::runUpdate, this, profiler.getTimer("update")));
		dedicatedThreads.push_back(std::thread(&Engine::runRendering, this, profiler.getTimer("rendering")));

		/* Make the remaining CPU threads generalized workers
		   after the main and dedicated ones. */
		OS::getThreadPool().initialize(OS::getPlatform().getLogicalCoreCount() - dedicatedThreads.size() - 1);

		std::shared_ptr<HiResTimer> mainLoopTimer = profiler.getTimer("main_loop");
		uint32_t mainLoopTargetFPS = 60;

		/* Main loop thread for Input, AudioSystem and HapticSystem. */
		while (this->state->isRunning())
		{
			mainLoopTimer->setStart();
			this->runInput(profiler.getTimer("input"));
			this->runAudio(profiler.getTimer("audio"));
			this->runHaptic(profiler.getTimer("haptic"));
			mainLoopTimer->setEnd();
			this->sleepThisThreadForRemainingTime(mainLoopTargetFPS, mainLoopTimer);
		}

		for (std::thread& thread : dedicatedThreads)
			thread.join();

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
	isInitializedOK = OS::initialize(
		"", 
		640, 
		480, 
		false, 
		this->argv[0], 
		this->renderingSystem->getRenderingContextName());

	if (isInitializedOK)
	{
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
	}
	else
	{
		this->state->setShuttingDown();
	}

	return isInitializedOK;
}

void Omnia::Engine::runInput(std::shared_ptr<HiResTimer> inputTimer)
{
	inputTimer->setStart();
	Input& input = OS::getInput();

	input.detectGameControllers();
	input.pollInputEvents();
	if (input.hasRequestedShutdown())
		this->state->setShuttingDown();
	if (input.hasRequestedRestart())
		this->state->setRestarting();

	inputTimer->setEnd();
}

void Omnia::Engine::runUpdate(std::shared_ptr<HiResTimer> updateTimer)
{
	while (1)
	{
		updateTimer->setStart();

		/* Placeholder until mutexes are added. */

		//std::shared_ptr<Scene> activeScene = this->application->getActiveScene();
		//const uint32_t msPerComputeUpdate = this->application->getConfiguration()->timeSettings.msPerComputeUpdate;

		//this->application->executeOnInputMethods();
		//this->application->executeOnStartMethods();
		//this->application->executeOnLogicFrameMethods();
		//this->uiSystem->process(activeScene);
		//this->aiSystem->process(activeScene);

		///* This calls the compute based Systems repeatedly until the accumulated
		//   lag milliseconds are depleted. This ensures compute operations
		//   are accurate to real-time, even when frames drop. */

		//this->animationSystem->setMsPerComputeUpdate(msPerComputeUpdate);
		//this->physicsSystem->setMsPerComputeUpdate(msPerComputeUpdate);

		////while (profiler.getLagCount() >= msPerComputeUpdate)
		////{
		////	this->application->executeOnComputeFrameMethods();
		////	this->animationSystem->process(activeScene);
		////	this->physicsSystem->process(activeScene);
		////	profiler.decrementLagCount(msPerComputeUpdate);
		////}

		//this->physicsSystem->onComputeEnd(activeScene);
		//this->application->executeOnOutputMethods();
		//this->application->executeOnFinishMethods();
		////profiler.incrementLagCount(profiler.getTimer("frame")->getDelta());
		updateTimer->setEnd();

		//this->sleepThisThreadForRemainingTime(msPerComputeUpdate * 2, updateTimer);
		this->sleepThisThreadForRemainingTime((1.0 / 0.008) / 2.0, updateTimer);
	}
}

void Omnia::Engine::runRendering(std::shared_ptr<HiResTimer> renderingTimer)
{
	while (1)
	{
		renderingTimer->setStart();
		/* Placeholder until mutexes are added. */

		//this->renderingSystem->process(this->application->getActiveScene());
		renderingTimer->setEnd();
		this->sleepThisThreadForRemainingTime(
			this->application->getConfiguration()->timeSettings.targetFPS,
			renderingTimer);
	}
}

void Omnia::Engine::runAudio(std::shared_ptr<HiResTimer> audioTimer)
{
	audioTimer->setStart();
	this->audioSystem->process(this->application->getActiveScene());
	audioTimer->setEnd();
}

void Omnia::Engine::runHaptic(std::shared_ptr<HiResTimer> hapticTimer)
{
	hapticTimer->setStart();
	this->hapticSystem->process(this->application->getActiveScene());
	hapticTimer->setEnd();
}

void Omnia::Engine::sleepThisThreadForRemainingTime(uint32_t targetFPS, std::shared_ptr<HiResTimer> runTimer)
{
	float targetFrameTime = 1000.0 / targetFPS;
	float runTime = runTimer->getDelta();
	OS::sleepThisThreadFor(targetFrameTime - runTime);
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
}