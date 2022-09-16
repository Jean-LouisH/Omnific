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
#include "utilities/constants.hpp"
#include "scene/assets/image.hpp"
#include <iostream>

#include <customization/class_registry/class_registry.hpp>

Omnia::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->argc = argc;
	this->argv = argv;
}

void Omnia::Engine::run()
{
	do
	{
		std::string dataDirectory = "data/";

		this->initialize();
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

		FileAccess& fileAccess = OS::getFileAccess();

		if (fileAccess.exists(bootFilepath))
		{
			Configuration::loadFromFile(bootFilepath);
			OS::getFileAccess().setDataDirectory(dataDirectory);

			Image image = Image(dataDirectory + Configuration::getInstance()->metadata.iconFilepath);
			OS::getWindow().changeIcon(image);

			std::string entrySceneFilepath = Configuration::getInstance()->metadata.entrySceneFilepath;
			if (fileAccess.exists(fileAccess.getDataDirectoryPath() + entrySceneFilepath))
			{
				std::shared_ptr<Scene> entryScene(new Scene(entrySceneFilepath));
				SceneStorage::addScene(Configuration::getInstance()->metadata.entrySceneFilepath, entryScene);
			}
		}

#ifdef DEBUG_CONSOLE_ENABLED
		std::cout << "\n\n\tOmnia Debug Console Enabled";
		std::cout << "\n\nPress '`' in-application to write to command line via console.";
		std::cout << "\n\nTo see the list of commands, enter 'commands'.";
		std::cout << "\n\n";
#endif

		if (Configuration::getInstance()->isLoaded)
		{
			OS::addGameControllerMappings(dataDirectory + "gamecontrollerdb.txt");
			Window& window = OS::getWindow();
			window.resize(Configuration::getInstance()->windowSettings.width, Configuration::getInstance()->windowSettings.height);
			window.changeTitle(Configuration::getInstance()->metadata.title.c_str());
			this->state = State::RUNNING_APPLICATION_WINDOWED;
			OS::getLogger().write("Loaded application project \"" + Configuration::getInstance()->metadata.title + "\" at: " + dataDirectory);
		}
		else
		{
			OS::showErrorBox(
				"Could not load game data", 
				"The game data is either missing or corrupted. Reinstall and try again");
			this->state = State::FINALIZING;
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
		while (this->isRunning())
		{
			mainThreadTimer->setStart();
			this->queryInput();
			mainThreadTimer->setEnd();
			this->sleepThisThreadForRemainingTime(mainThreadTargetFPS, mainThreadTimer);
		}

		for (std::thread& thread : dedicatedThreads)
			thread.join();

		this->finalize();
	} while (this->state == State::RESTARTING);
}

bool Omnia::Engine::isRunning()
{
	return  (this->state == State::RUNNING_APPLICATION_FULLSCREEN ||
		this->state == State::RUNNING_APPLICATION_FULLSCREEN_DESKTOP ||
		this->state == State::RUNNING_APPLICATION_WINDOWED);
}

void Omnia::Engine::initialize()
{
	ClassRegistry::initialize();

	/* Load Systems from the ClassRegistry */
	for (auto it : ClassRegistry::queryAll<System>())
	{
		std::shared_ptr<System> system = std::dynamic_pointer_cast<System>(it.second);

		if (system->isThreadType(ThreadType::UPDATE))
			this->updateSystems.emplace(it.first, std::dynamic_pointer_cast<System>(std::shared_ptr<Registerable>(system->instance())));
		else if (system->isThreadType(ThreadType::OUTPUT))
			this->outputSystems.emplace(it.first, std::dynamic_pointer_cast<System>(std::shared_ptr<Registerable>(system->instance())));
	}

	if (this->state != State::RESTARTING)
		this->state = State::INITIALIZING;

	std::vector<std::string> commandLineArguments;

	for (int i = 0; i < this->argc; i++)
		commandLineArguments.push_back(this->argv[i]);

	/* Hardware abstraction layer initialization. */
	OS::initialize(commandLineArguments);

	Platform& platform = OS::getPlatform();
	Logger& logger = OS::getLogger();

	logger.write("Retrieved Logical Core Count: " + std::to_string(platform.getLogicalCoreCount()));
	logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.getL1CacheLineSize()) + " B");
	logger.write("Retrieved OS Name: " + platform.getOSName());
	logger.write("Retrieved System RAM: " + std::to_string(platform.getSystemRAM()) + " MB");
}

void Omnia::Engine::queryInput()
{
	Input& input = OS::getInput();
	input.detectGameControllers();
	input.pollInputEvents();
	if (input.hasRequestedShutdown())
		this->state = State::FINALIZING;
	if (input.hasRequestedRestart())
		this->state = State::RESTARTING;
}

void Omnia::Engine::runUpdate(std::shared_ptr<HiResTimer> updateProcessTimer)
{
	for (auto updateSystem : this->updateSystems)
		updateSystem.second->initialize();

	Profiler& profiler = OS::getProfiler();
	HiResTimer updateFrameTimer;

	while (this->isRunning())
	{
		updateProcessTimer->setStart();
		updateFrameTimer.setStart();

		std::shared_ptr<Scene> activeScene = SceneStorage::getActiveScene();
		const uint32_t msPerComputeUpdate = Configuration::getInstance()->timeSettings.msPerComputeUpdate;

#ifdef DEBUG_CONSOLE_ENABLED
		if (OS::getInput().hasRequestedCommandLine())
		{
			std::string command;

			OS::getWindow().hide();
			std::cout << std::endl << ">";
			std::cin.ignore(1, '\n');
			std::getline(std::cin, command);
			this->getSystem<ScriptingSystem>()->executeCommand(command);
			OS::getWindow().show();
		}
#endif

		if (OS::getInput().getHasDetectedInputChanges())
			for (auto updateSystem : this->updateSystems)
				updateSystem.second->onInput(activeScene);

		if (SceneStorage::hasActiveSceneChanged())
			this->getSystem<ScriptingSystem>()->loadScriptModules(SceneStorage::getActiveScene());

		for (auto updateSystem : this->updateSystems)
			updateSystem.second->onStart(activeScene);

		for (auto updateSystem : this->updateSystems)
			updateSystem.second->onEarly(activeScene);

		for (auto updateSystem : this->updateSystems)
			updateSystem.second->onLogic(activeScene);

		/* This calls the compute based Systems repeatedly until the accumulated
		   lag milliseconds are depleted. This ensures compute operations
		   are accurate to real-time, even when frames drop. */

		while (profiler.getLagCount() >= msPerComputeUpdate && this->isRunning())
		{
			for (auto updateSystem : this->updateSystems)
				updateSystem.second->onCompute(activeScene);
			profiler.decrementLagCount(msPerComputeUpdate);
		}

		for (auto updateSystem : this->updateSystems)
			updateSystem.second->onLate(activeScene);

		for (auto updateSystem : this->updateSystems)
			updateSystem.second->onFinish(activeScene);

		for (auto it : activeScene->getSceneTrees())
			it.second->getEventBus()->clear();

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
	for (auto outputSystem : this->outputSystems)
		outputSystem.second->initialize();

	/* The RenderingSystem only reads Scene data. */
	while (this->isRunning())
	{
		outputProcessTimer->setStart();
		for (auto outputSystem : this->outputSystems)
			outputSystem.second->onLate(SceneStorage::getActiveScene());
		outputProcessTimer->setEnd();
		this->sleepThisThreadForRemainingTime(
			Configuration::getInstance()->timeSettings.targetFPS,
			outputProcessTimer);
	}
}

void Omnia::Engine::sleepThisThreadForRemainingTime(uint32_t targetFPS, std::shared_ptr<HiResTimer> runTimer)
{
	float targetFrameTime = 1000.0 / targetFPS;
	float runTime = runTimer->getDelta();
	OS::sleepThisThreadFor(targetFrameTime - runTime);
}

void Omnia::Engine::finalize()
{
	OS::finalize();

	for (auto updateSystem : this->updateSystems)
		updateSystem.second.reset();

	for (auto outputSystem : this->outputSystems)
		outputSystem.second.reset();
}