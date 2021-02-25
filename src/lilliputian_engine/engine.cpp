#include "engine.hpp"

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->profiler = new Profiler();
	this->profiler->run().setStart();
	this->profiler->benchmark().setStart();
	this->argc = argc;
	this->argv = argv;
}

void Lilliputian::Engine::run()
{
	do
	{
		if (this->initialize())
		{
			this->game->initialize();
			BootConfiguration configuration = this->game->configuration();

			if (configuration.isLoaded)
			{
				Window window = this->os->window();
				window.resizeWindow(configuration.windowWidth, configuration.windowHeight);
				window.changeTitle(configuration.gameTitle.c_str());
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

		while (this->state.isRunning())
		{
			this->profiler->frame().setStart();
			this->profiler->process().setStart();
			this->input();
			this->update();
			this->output();
			this->benchmark();
			this->profiler->process().setEnd();
			this->sleep();
			this->profiler->frame().setEnd();
			this->profiler->run().setEnd();
			this->profiler->incrementFrameCount();
		}

		this->game->deinitialize();
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
		this->os = new OS("", 640, 480, false, this->argv[0]);
		this->game = new Game(this->os, this->profiler);
		this->aiSystem = new AISystem();
		this->animationSystem = new AnimationSystem();
		this->audioSystem = new AudioSystem();
		this->hapticSystem = new HapticSystem();
		this->physicsSystem = new PhysicsSystem();
		this->renderingSystem = new RenderingSystem(this->os->window());
		this->uiSystem = new UISystem();

		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Lilliputian::Engine::input()
{
	this->profiler->input().setStart();
	HumanInterfaceDevices hid = this->os->hid();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state.setShuttingDown();

	this->profiler->input().setEnd();
}

void Lilliputian::Engine::update()
{
	this->profiler->update().setStart();
	Scene activeScene = this->game->getActiveScene();
	uint32_t msPerComputeUpdate = this->game->configuration().msPerComputeUpdate;

	this->game->executeOnInputMethods();
	this->game->executeOnStartMethods();
	this->game->executeOnFrameMethods();
	this->uiSystem->process(activeScene, this->os->hid());
	this->aiSystem->process(activeScene);

	while (this->profiler->getLag_ms() >= msPerComputeUpdate)
	{
		this->game->executeOnComputeMethods();
		this->animationSystem->process(activeScene);
		this->physicsSystem->process(activeScene);
		this->profiler->decrementLagCount(msPerComputeUpdate);
	}

	this->game->executeOnLateMethods();
	this->game->executeOnFinalMethods();
	this->profiler->incrementLagCount(this->profiler->frame().getDelta_ns() / NS_IN_MS);
	this->profiler->update().setEnd();
}

void Lilliputian::Engine::output()
{
	this->profiler->output().setStart();

	if (this->renderingSystem != nullptr)
		this->renderingSystem->process(this->game->getActiveScene());

	if (this->audioSystem != nullptr)
		this->audioSystem->process(this->game->getActiveScene());

	if (this->hapticSystem != nullptr)
		this->hapticSystem->process(this->game->getActiveScene(), this->os->hid());

	this->profiler->output().setEnd();
}

void Lilliputian::Engine::benchmark()
{
#ifdef _DEBUG
	uint32_t FPSUpdateSeconds = 1;

	if (this->profiler->benchmark().getDelta_ns() / NS_IN_MS >= (FPSUpdateSeconds * MS_IN_S))
	{
		this->profiler->benchmark().setStart();
		String FPSString = std::to_string(this->profiler->getFPS());
		String frameUtilizationString =
			std::to_string((int)(((double)this->profiler->process().getDelta_ns() / (double)this->profiler->frame().getDelta_ns()) * 100));
		this->os->window().changeTitle((this->game->configuration().gameTitle + " ->" +
			" FPS: " + FPSString +
			", Frame Time Utilization: " + frameUtilizationString + "%").c_str()
		);
	}
#endif
	this->profiler->benchmark().setEnd();
}

void Lilliputian::Engine::sleep()
{
	float targetFrameTime_ms = 1000.0 / this->game->configuration().targetFPS;
	float processTime_ms = this->profiler->process().getDelta_ns() / NS_IN_MS;
	this->os->window().sleep(targetFrameTime_ms - processTime_ms);
}

void Lilliputian::Engine::shutdown()
{
	delete this->os;
	delete this->game;
	delete this->aiSystem;
	delete this->animationSystem;
	delete this->audioSystem;
	delete this->hapticSystem;
	delete this->physicsSystem;
	delete this->renderingSystem;
	delete this->uiSystem;
	delete this->profiler;
}