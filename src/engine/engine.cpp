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
#include "core/utilities/constants.hpp"
#include "core/assets/image.hpp"
#include <iostream>

#include <customization/class_registry/class_registry.hpp>

void Omnia::Engine::run(
	int argc,
	char* argv[])
{
	OS::initialize(argc, argv);
	Logger& logger = OS::getLogger();
	logger.write("Initializing Omnia Engine...");
	ClassRegistry::initialize();

	do
	{
		Profiler& profiler = OS::getProfiler();

		this->initialize();

		if (this->state != State::FINALIZING)
		{

			/* These timers persist throughout Engine runtime and
			   keep track of elapsed times in nanoseconds. */
			profiler.addTimer(MAIN_THREAD_TIMER_NAME, false);
			profiler.addTimer(UPDATE_THREAD_TIMER_NAME, false);
			profiler.addTimer(OUTPUT_THREAD_TIMER_NAME, false);

			/* Engine threading uses a hybrid of dedicated threads
			   for deadline sensitive tasks and a thread pool for
			   general parallelizable tasks. */

			std::vector<std::thread> dedicatedThreads;
			dedicatedThreads.push_back(std::thread(&Engine::runUpdateLoop, this, profiler.getTimer(UPDATE_THREAD_TIMER_NAME)));
			dedicatedThreads.push_back(std::thread(&Engine::runOutputLoop, this, profiler.getTimer(OUTPUT_THREAD_TIMER_NAME)));

			/* Make the remaining CPU threads generalized workers
			   after the main and dedicated ones. */
			OS::getThreadPool().initialize(OS::getPlatform().getLogicalCoreCount() - dedicatedThreads.size() - 1);

			logger.write("Engine loops currently running...");

			/* Input loop must run on the main thread. */
			this->runInputLoop(profiler.getTimer("main_thread"));

			logger.write("Finalizing Engine loops...");
			for (std::thread& thread : dedicatedThreads)
				thread.join();

			this->finalize();
		}
	} while (this->state == State::RESTARTING);
}

void Omnia::Engine::initialize()
{
	Logger& logger = OS::getLogger();

	if (this->state != State::RESTARTING)
		this->state = State::INITIALIZING;

	logger.write("Loading Systems from ClassRegistry...");

	/* Load Systems from the ClassRegistry */
	for (auto it : ClassRegistry::queryAll<System>())
	{
		std::shared_ptr<System> system = std::dynamic_pointer_cast<System>(it.second);

		this->systems.emplace(
			it.first,
			std::dynamic_pointer_cast<System>(std::shared_ptr<Registerable>(system->instance())));
	}

	logger.write("Querying Platform...");

	Platform& platform = OS::getPlatform();

	logger.write("Retrieved Logical Core Count: " + std::to_string(platform.getLogicalCoreCount()));
	logger.write("Retrieved L1 Cache Line Size: " + std::to_string(platform.getL1CacheLineSize()) + " B");
	logger.write("Retrieved OS Name: \"" + platform.getOSName() + "\"");
	logger.write("Retrieved System RAM: " + std::to_string(platform.getSystemRAM()) + " MB");

	std::string dataDirectory = DATA_DIRECTORY;

#ifdef _DEBUG
	while (true)
	{
		OS::getWindow().hide();
		std::cout << "\n\nChoose data project to load:" << std::endl;
		std::cout << "0. Custom" << std::endl;
		std::cout << "1. Demos" << std::endl;
		std::cout << "2. Editor" << std::endl;
		std::cout << "3. Debug" << std::endl;
		std::cout << "\n-> #";

		std::string inputString;
		std::cin >> inputString;
		OS::getWindow().show();

		if (inputString == "0")
		{
			break;
		}
		else if (inputString == "1")
		{
			dataDirectory = DEBUG_DEMO_DATA_DIRECTORY;
			break;
		}
		else if (inputString == "2")
		{
			dataDirectory = DEBUG_EDITOR_DATA_DIRECTORY;
			break;
		}
		else if (inputString == "3")
		{
			dataDirectory = DEBUG_DEBUG_DATA_DIRECTORY;
			break;
		}
		else
		{
			std::cout << "Error: Option \"" + inputString + "\" not listed." << std::endl;
		}
	}
#endif
	std::string bootFilepath = dataDirectory + BOOT_FILE_NAME;
	FileAccess& fileAccess = OS::getFileAccess();

	if (fileAccess.exists(bootFilepath))
	{
		Configuration::loadFromFile(bootFilepath);
		OS::getFileAccess().setDataDirectory(dataDirectory);
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\n";
#endif

	if (Configuration::getInstance()->isLoaded)
	{
		OS::addGameControllerMappings(dataDirectory + GAME_CONTROLLER_DATABASE_FILE_NAME);
		Window& window = OS::getWindow();
		window.resize(Configuration::getInstance()->windowSettings.width, Configuration::getInstance()->windowSettings.height);
		window.changeTitle(Configuration::getInstance()->metadata.title.c_str());
		this->state = State::RUNNING;
		logger.write("Loaded application project \"" + Configuration::getInstance()->metadata.title + "\" at: " + dataDirectory);
	}
	else
	{
		OS::showErrorBox(
			"Could not load game data",
			"The game data is either missing or corrupted. Reinstall and try again");
		logger.write("Shutting down Engine due to error in loading Configuration.");
		this->state = State::FINALIZING;
	}

}

void Omnia::Engine::runInputLoop(std::shared_ptr<HiResTimer> inputProcessTimer)
{
	Input& input = OS::getInput();

	while (this->state == State::RUNNING)
	{
		inputProcessTimer->setStart();
		input.detectGameControllers();
		input.pollInputEvents();
		if (input.hasRequestedShutdown())
			this->state = State::FINALIZING;
		if (input.hasRequestedRestart())
			this->state = State::RESTARTING;
		inputProcessTimer->setEnd();
		this->sleepThisThreadForRemainingTime(INPUT_TARGET_FPS, inputProcessTimer);
	}
}

void Omnia::Engine::runUpdateLoop(std::shared_ptr<HiResTimer> updateProcessTimer)
{
	FileAccess& fileAccess = OS::getFileAccess();
	std::string entrySceneFilepath = Configuration::getInstance()->metadata.entrySceneFilepath;

	if (fileAccess.exists(fileAccess.getDataDirectoryPath() + entrySceneFilepath))
		SceneStorage::addAndChangeToScene(std::shared_ptr<Scene>(new Scene(entrySceneFilepath)));

	for (auto system : this->systems)
		system.second->initialize();

	Profiler& profiler = OS::getProfiler();
	profiler.addTimer(UPDATE_FRAME_TIMER_NAME);
	std::shared_ptr<HiResTimer> updateFrameTimer = profiler.getTimer(UPDATE_FRAME_TIMER_NAME);

	while (this->state == State::RUNNING)
	{
		updateProcessTimer->setStart();
		updateFrameTimer->setStart();

		std::shared_ptr<Scene> activeScene = SceneStorage::getActiveScene();
		uint32_t msPerComputeUpdate = Configuration::getInstance()->timeSettings.msPerComputeUpdate;

		if (msPerComputeUpdate > MAXIMUM_MS_PER_COMPUTE_UPDATE)
			msPerComputeUpdate = MAXIMUM_MS_PER_COMPUTE_UPDATE;

		if (OS::getInput().getHasDetectedInputChanges())
			for (auto system : this->systems)
				system.second->onInput(activeScene);

		for (auto system : this->systems)
			system.second->onStart(activeScene);

		for (auto it : activeScene->getSceneLayers())
			it.second->clearStartEntityQueue();

		for (auto system : this->systems)
			system.second->onEarly(activeScene);

		for (auto system : this->systems)
			system.second->onLogic(activeScene);

		/* This calls the compute based Systems repeatedly until the accumulated
		   lag milliseconds are depleted. This ensures compute operations
		   are accurate to real-time, even when frames drop. */

		while (profiler.getLagCount() >= msPerComputeUpdate && 
			this->state == State::RUNNING)
		{
			for (auto system : this->systems)
				system.second->onCompute(activeScene);
			profiler.decrementLagCount(msPerComputeUpdate);
		}

		for (auto system : this->systems)
			system.second->onLate(activeScene);

		for (auto system : this->systems)
			system.second->onFinish(activeScene);

		for (auto it : activeScene->getSceneLayers())
			it.second->clearFinishEntityQueue();

		for (auto it : activeScene->getSceneLayers())
			it.second->getEventBus()->clear();

		SceneStorage::finalizeUpdate();

		profiler.incrementLagCount(updateFrameTimer->getDelta());
		updateProcessTimer->setEnd();

		this->sleepThisThreadForRemainingTime(
			UPDATE_TARGET_FPS,
			updateProcessTimer);
		updateFrameTimer->setEnd();
	}

	for (auto system : this->systems)
		system.second->finalize();
}

void Omnia::Engine::runOutputLoop(std::shared_ptr<HiResTimer> outputProcessTimer)
{
	for (auto system : this->systems)
		system.second->initializeOutput();

	/* Read only for Scene data. */
	while (this->state == State::RUNNING)
	{
		outputProcessTimer->setStart();

		std::shared_ptr<Scene> activeScene = SceneStorage::getActiveScene();

		for (auto system : this->systems)
			system.second->onOutput(activeScene);

		outputProcessTimer->setEnd();
		this->sleepThisThreadForRemainingTime(
			Configuration::getInstance()->timeSettings.targetFPS,
			outputProcessTimer);
	}

	for (auto system : this->systems)
		system.second->finalizeOutput();
}

void Omnia::Engine::sleepThisThreadForRemainingTime(uint32_t targetFPS, std::shared_ptr<HiResTimer> runTimer)
{
	float targetFrameTime = 1000.0 / targetFPS;
	float runTime = runTimer->getDelta();
	OS::sleepThisThreadFor(targetFrameTime - runTime);
}

void Omnia::Engine::finalize()
{
	SceneStorage::clearScenes();
	OS::getThreadPool().finalize();
}