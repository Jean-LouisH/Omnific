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

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->argc = argc;
	this->argv = argv;
}

void Lilliputian::Engine::run()
{
	do
	{
		if (this->initialize())
		{
			this->application->initialize();
			Configuration configuration = this->application->getConfiguration();

			if (configuration.isLoaded)
			{
				OS::addGameControllerMappings();
				Window window = OS::getWindow();
				window.resize(configuration.windowSettings.windowWidth, configuration.windowSettings.windowHeight);
				window.changeTitle(configuration.metadata.gameTitle.c_str());
				this->state.setRunningApplicationWindowed();
			}
			else
			{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
					"Could not load game data",
					"The game data is either missing or corrupted. Reinstall and try again",
					NULL
				);
				this->state.setShuttingDown();
			}

		}

		Profiler& profiler = OS::getProfiler();

		while (this->state.isRunning())
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
	} while (this->state.isRestarting());
}

bool Lilliputian::Engine::initialize()
{
	bool isInitializedOK = false;

	this->state.setInitializing();

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		SDL_Log(
			"SDL could not initialize because: %s",
			SDL_GetError);
		this->state.setShuttingDown();
	}
	else
	{
		OS::initialize("", 640, 480, false, this->argv[0]);

		Profiler& profiler = OS::getProfiler();
		profiler.getBenchmarkTimer().setStart();

		this->application = new Application();
		this->aiSystem = new AISystem();
		this->animationSystem = new AnimationSystem();
		this->audioSystem = new AudioSystem();
		this->hapticSystem = new HapticSystem();
		this->physicsSystem = new PhysicsSystem();
		this->renderingSystem = new RenderingSystem(OS::getWindow());
		this->uiSystem = new UISystem();

		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Lilliputian::Engine::input()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getInputTimer().setStart();
	HumanInterfaceDevices& hid = OS::getHid();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state.setShuttingDown();

	profiler.getInputTimer().setEnd();
}

void Lilliputian::Engine::update()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getUpdateTimer().setStart();
	Scene& activeScene = this->application->getActiveScene();
	const uint32_t msPerComputeUpdate = this->application->getConfiguration().timeSettings.msPerComputeUpdate;

	this->application->executeOnInputMethods();
	this->application->executeOnStartMethods();
	this->application->executeOnFrameMethods();
	this->uiSystem->process(activeScene, OS::getHid());
	this->aiSystem->process(activeScene);

	while (profiler.getLag_ms() >= msPerComputeUpdate)
	{
		this->application->executeOnComputeMethods();
		this->animationSystem->process(activeScene);
		this->physicsSystem->process(activeScene, msPerComputeUpdate);
		profiler.decrementLagCount(msPerComputeUpdate);
	}

	this->application->executeOnLateMethods();
	this->application->executeOnFinalMethods();
	profiler.incrementLagCount(profiler.getFrameTimer().getDelta_ns() / NS_IN_MS);
	profiler.getUpdateTimer().setEnd();
}

void Lilliputian::Engine::output()
{
	Profiler& profiler = OS::getProfiler();
	profiler.getOutputTimer().setStart();

	this->renderingSystem->process(this->application->getActiveScene());
	this->audioSystem->process(this->application->getActiveScene());
	this->hapticSystem->process(this->application->getActiveScene(), OS::getHid());

	profiler.getOutputTimer().setEnd();
}

void Lilliputian::Engine::benchmark()
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
		OS::getWindow().changeTitle((this->application->getConfiguration().metadata.gameTitle + " (DEBUG) ->" +
			" FPS: " + FPSString).c_str()
		);
	}
#endif
	profiler.getBenchmarkTimer().setEnd();
}

void Lilliputian::Engine::sleep()
{
	Profiler& profiler = OS::getProfiler();
	float targetFrameTime_ms = 1000.0 / this->application->getConfiguration().timeSettings.targetFPS;
	float processTime_ms = profiler.getProcessTimer().getDelta_ns() / NS_IN_MS;
	OS::getWindow().sleep(targetFrameTime_ms - processTime_ms);
}

void Lilliputian::Engine::shutdown()
{
	delete this->application;
	delete this->aiSystem;
	delete this->animationSystem;
	delete this->audioSystem;
	delete this->hapticSystem;
	delete this->physicsSystem;
	delete this->renderingSystem;
	delete this->uiSystem;
}