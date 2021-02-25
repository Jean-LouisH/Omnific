#include "engine.hpp"

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->profiler = new Profiler();
	this->argc = argc;
	this->argv = argv;
}

Lilliputian::Engine::~Engine()
{

}

void Lilliputian::Engine::run()
{
	do
	{
		if (this->initialize())
		{
			this->game->initialize();
			BootConfiguration configuration = this->game->configuration();
			Window window = this->os->window();
			window.resizeWindow(configuration.windowWidth, configuration.windowHeight);
			window.changeTitle(configuration.gameTitle.c_str());
			this->state.setRunningApplicationWindowed();
		}

		while (this->state.isRunning())
		{
			this->profiler->frame().setStart();
			this->profiler->process().setStart();
			this->input();
			this->logic();
			this->compute();
			this->output();
			this->profiler->process().setEnd();
			this->sleep();
			this->profiler->frame().setEnd();
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

		isInitializedOK = true;
	}

	return isInitializedOK;
}

void Lilliputian::Engine::input()
{
	HumanInterfaceDevices hid = this->os->hid();

	hid.detectGameControllers();
	hid.pollInputEvents();
	if (hid.hasRequestedShutdown())
		this->state.setShuttingDown();
}

void Lilliputian::Engine::logic()
{
	this->game->executeOnStartMethods();
	this->game->executeOnInputMethods();
	this->game->executeOnFrameMethods();
}

void Lilliputian::Engine::compute()
{
	uint32_t msPerComputeUpdate = this->game->configuration().msPerComputeUpdate;

	while (this->profiler->getLag_ms() >= msPerComputeUpdate)
	{
		this->game->executeOnComputeMethods();
		this->profiler->decrementLagCount(msPerComputeUpdate);
	}

	this->game->executeOnLateMethods();
	this->game->executeOnFinalMethods();
	this->profiler->incrementLagCount(this->profiler->frame().getDelta_ns() / NS_IN_MS);
}

void Lilliputian::Engine::output()
{
	if (this->renderingSystem != nullptr)
		this->renderingSystem->process(this->game->getActiveScene());

	if (this->audioSystem != nullptr)
		this->audioSystem->process(this->game->getActiveScene());

	if (this->hapticSystem != nullptr)
		this->hapticSystem->process(this->game->getActiveScene(), this->os->hid());
}

void Lilliputian::Engine::sleep()
{
	float targetFrameTime_ms = 1000.0 / this->game->configuration().targetFPS;
	float processTime_ms = this->profiler->process().getDelta_ns() / 1000.0;
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
	delete this->profiler;
}