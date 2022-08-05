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
#include "boot_loader.hpp"
#include "utilities/constants.hpp"
#include "scene/assets/image.hpp"
#include <iostream>

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
		std::string dataDirectory = "data/";

		/* On successful initialization */
		if (this->initialize())
		{
			BootLoader bootLoader;
			std::shared_ptr<Scene> entryScene;
#ifdef _DEBUG
			OS::getWindow().hide();
			std::cout << "\n\nChoose data project to load:" << std::endl;
			std::cout << "1. Demos" << std::endl;
			std::cout << "2. Editor" << std::endl;
			std::cout << "3. Debug" << std::endl;
			std::cout << "\n-> #";

			std::string inputString;
			std::cin >> inputString;
			OS::getWindow().show();

			if (inputString == "1")
				dataDirectory = DEBUG_DEMO_DATA_FILEPATH;
			else if (inputString == "2")
				dataDirectory = DEBUG_EDITOR_DATA_FILEPATH;
			else if (inputString == "3")
				dataDirectory = DEBUG_DEBUG_DATA_FILEPATH;
#endif
			std::string bootFilename = "boot.yml";
			std::string bootFilepath = dataDirectory + bootFilename;

			if (OS::getFileAccess().exists(bootFilepath))
			{
				this->configuration = bootLoader.loadFromFile(bootFilepath);
				OS::getFileAccess().setDataDirectory(dataDirectory);
				this->sceneSerializer = std::shared_ptr<SceneSerializer>(new SceneSerializer(dataDirectory));
				this->sceneStorage = std::shared_ptr<SceneStorage>(new SceneStorage());

				Image image = Image(dataDirectory + this->configuration->metadata.iconFilepath);
				OS::getWindow().changeIcon(image);

				if (this->sceneSerializer->doesSceneExist(this->configuration->metadata.entrySceneFilepath))
				{
					entryScene = this->sceneSerializer->deserialize(this->configuration->metadata.entrySceneFilepath);
					this->sceneStorage->addScene(this->configuration->metadata.entrySceneFilepath, entryScene);
					this->scriptingSystem->setSceneSerializer(this->sceneSerializer);
					this->scriptingSystem->setSceneStorage(this->sceneStorage);
				}
			}
			else
			{
				this->configuration = std::shared_ptr<Configuration>(new Configuration());
				this->configuration->isLoaded = false;
			}

#ifdef DEBUG_CONSOLE_ENABLED
			std::cout << "\n\n\tOmnia Debug Console Enabled";
			std::cout << "\n\nPress '`' in-application to write to command line via console.";
			std::cout << "\n\nTo see the list of commands, enter 'commands'.";
			std::cout << "\n\n";
#endif

			if (this->configuration->isLoaded)
			{
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
		profiler.addTimer("main_thread");
		profiler.addTimer("update_thread");
		profiler.addTimer("output_thread");

		/* Engine threading uses a hybrid of dedicated threads
		   for deadline sensitive tasks and a thread pool for 
		   general parallelizable tasks. */

		std::vector<std::thread> dedicatedThreads;
		dedicatedThreads.push_back(std::thread(&Engine::runUpdate, this, profiler.getTimer("update_thread")));
		dedicatedThreads.push_back(std::thread(&Engine::runOutput, this, profiler.getTimer("output_thread")));

		/* Make the remaining CPU threads generalized workers
		   after the main and dedicated ones. */
		OS::getThreadPool().initialize(OS::getPlatform().getLogicalCoreCount() - dedicatedThreads.size() - 1);

		std::shared_ptr<HiResTimer> mainThreadTimer = profiler.getTimer("main_thread");
		uint32_t mainThreadTargetFPS = 60;

		/* Main loop thread for Input. */
		while (this->state->isRunning())
		{
			mainThreadTimer->setStart();
			this->queryInput();
			mainThreadTimer->setEnd();
			this->sleepThisThreadForRemainingTime(mainThreadTargetFPS, mainThreadTimer);
		}

		for (std::thread& thread : dedicatedThreads)
			thread.join();

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
	this->scriptingSystem = std::unique_ptr<ScriptingSystem>(new ScriptingSystem());
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
		Platform& platform = OS::getPlatform();
		Logger& logger = OS::getLogger();

		logger.write("Retrieved Logical Core Count: " + std::to_string(platform.getLogicalCoreCount()));
		logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.getL1CacheLineSize()) + " B");
		logger.write("Retrieved OS Name: " + platform.getOSName());
		logger.write("Retrieved System RAM: " + std::to_string(platform.getSystemRAM()) + " MB");
	}
	else
	{
		this->state->setShuttingDown();
	}

	return isInitializedOK;
}

void Omnia::Engine::queryInput()
{
	Input& input = OS::getInput();
	input.detectGameControllers();
	input.pollInputEvents();
	if (input.hasRequestedShutdown())
		this->state->setShuttingDown();
	if (input.hasRequestedRestart())
		this->state->setRestarting();
}

void Omnia::Engine::runUpdate(std::shared_ptr<HiResTimer> updateProcessTimer)
{
	this->aiSystem->initialize();
	this->animationSystem->initialize();
	this->physicsSystem->initialize();
	this->uiSystem->initialize();
	this->scriptingSystem->initialize();

	Profiler& profiler = OS::getProfiler();
	HiResTimer updateFrameTimer;

	while (this->state->isRunning())
	{
		updateProcessTimer->setStart();
		updateFrameTimer.setStart();

		std::shared_ptr<Scene> activeScene = this->sceneStorage->getActiveScene();
		const uint32_t msPerComputeUpdate = this->configuration->timeSettings.msPerComputeUpdate;

#ifdef DEBUG_CONSOLE_ENABLED
		if (OS::getInput().hasRequestedCommandLine())
		{
			std::string command;

			OS::getWindow().hide();
			std::cout << std::endl << ">";
			std::cin.ignore(1, '\n');
			std::getline(std::cin, command);
			this->scriptingSystem->executeCommand(command);
			OS::getWindow().show();
		}
#endif

		if (OS::getInput().getHasDetectedInputChanges())
			this->scriptingSystem->executeOnInputMethods(activeScene);

		if (this->sceneStorage->hasActiveSceneChanged())
			this->scriptingSystem->loadScriptModules(this->sceneStorage->getActiveScene());

		this->scriptingSystem->executeOnStartMethods(activeScene);
		this->scriptingSystem->executeOnLogicFrameMethods(activeScene);
		this->uiSystem->process(activeScene);
		this->aiSystem->process(activeScene);

		/* This calls the compute based Systems repeatedly until the accumulated
		   lag milliseconds are depleted. This ensures compute operations
		   are accurate to real-time, even when frames drop. */

		this->animationSystem->setMsPerComputeUpdate(msPerComputeUpdate);
		this->physicsSystem->setMsPerComputeUpdate(msPerComputeUpdate);

		while (profiler.getLagCount() >= msPerComputeUpdate && this->state->isRunning())
		{
			this->scriptingSystem->executeOnComputeFrameMethods(activeScene);
			this->animationSystem->process(activeScene);
			this->physicsSystem->process(activeScene);
			profiler.decrementLagCount(msPerComputeUpdate);
		}

		this->physicsSystem->onComputeEnd(activeScene);
		this->scriptingSystem->executeOnOutputMethods(activeScene);
		this->scriptingSystem->executeOnFinishMethods(activeScene);

		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = activeScene->getSceneTrees();
		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
			it->second->getEventBus()->clear();

		profiler.incrementLagCount(updateFrameTimer.getDelta());
		updateProcessTimer->setEnd();

		//this->sleepThisThreadForRemainingTime(msPerComputeUpdate * 2, updateTimer);
		this->sleepThisThreadForRemainingTime((1.0 / 0.008) / 2.0, updateProcessTimer);
		updateFrameTimer.setEnd();
	}
}

void Omnia::Engine::runOutput(std::shared_ptr<HiResTimer> outputProcessTimer)
{
	/* Initializes RenderingContext on the same
	   thread as object generators, as required. */
	this->renderingSystem->initialize();
	this->audioSystem->initialize();
	this->hapticSystem->initialize();

	/* The RenderingSystem only reads Scene data. */
	while (this->state->isRunning())
	{
		outputProcessTimer->setStart();
		this->renderingSystem->process(this->sceneStorage->getActiveScene());
		this->audioSystem->process(this->sceneStorage->getActiveScene());
		this->hapticSystem->process(this->sceneStorage->getActiveScene());
		outputProcessTimer->setEnd();
		this->sleepThisThreadForRemainingTime(
			this->configuration->timeSettings.targetFPS,
			outputProcessTimer);
	}
}

void Omnia::Engine::sleepThisThreadForRemainingTime(uint32_t targetFPS, std::shared_ptr<HiResTimer> runTimer)
{
	float targetFrameTime = 1000.0 / targetFPS;
	float runTime = runTimer->getDelta();
	OS::sleepThisThreadFor(targetFrameTime - runTime);
}

void Omnia::Engine::shutdown()
{
	OS::deinitialize();
	this->aiSystem.reset();
	this->animationSystem.reset();
	this->audioSystem.reset();
	this->hapticSystem.reset();
	this->physicsSystem.reset();
	this->renderingSystem.reset();
	this->scriptingSystem.reset();
	this->uiSystem.reset();
}