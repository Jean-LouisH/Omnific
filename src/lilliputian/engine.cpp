#include "engine.hpp"

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[])
{
	this->profiler = new Profiler();
	this->threadPool = new ThreadPool();
	this->commandArguments = argv;
	this->configuration.msPerComputeUpdate = 8;
	this->configuration.targetFPS = 60;
	this->configuration.windowHeight = 640;
	this->configuration.windowWidth = 480;
	this->configuration.isStartingFullscreen = false;
}

Lilliputian::Engine::~Engine()
{

}

void Lilliputian::Engine::run()
{
	do
	{
		if (this->initializeSystems())
		{
			this->state.setRunningApplicationWindowed();
			this->loadConfiguration(this->game->initialize());
		}

		while (this->state.isRunning())
		{
			this->profiler->process().setStart();
			this->input();
			this->logic();
			this->compute();
			this->output();
			this->profiler->process().setEnd();
			this->sleep();
			this->profiler->frameCount++;
		}

		this->game->deinitialize();
		this->shutdown();
	} while (this->state.isRestarting());
}

bool Lilliputian::Engine::initializeSystems()
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
		this->os = new OS(
			this->configuration.gameTitle.c_str(),
			this->configuration.windowHeight,
			this->configuration.windowWidth,
			this->configuration.isStartingFullscreen,
			this->commandArguments[0]);

		this->game = new Game(this->os, this->profiler);

		for (int i = 0; i < this->scriptCompilerCallbacks.size(); i++)
		{
			if (this->scriptCompilerCallbacks.at(i) != nullptr)
				this->scriptCompilerCallbacks.at(i)(this->game->getScriptRegistry());
		}

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

void Lilliputian::Engine::loadConfiguration(EngineConfiguration configuration)
{
	this->os->window().resizeWindow(configuration.windowWidth, configuration.windowHeight);
	this->os->window().changeTitle(configuration.gameTitle.c_str());
}

void Lilliputian::Engine::input()
{
	this->os->hid().detectGameControllers();
	this->os->hid().pollInputEvents();
	if (this->os->hid().hasRequestedShutdown())
		this->state.setShuttingDown();
}

void Lilliputian::Engine::logic()
{
	this->game->executeStartLogic();
	this->game->executeInputLogic();
	this->game->executeFrameLogic();
}

void Lilliputian::Engine::compute()
{

	this->game->executeLateLogic();
	this->game->executeFinalLogic();
}

void Lilliputian::Engine::output()
{
	if (this->renderingSystem != NULL)
		this->renderingSystem->process(this->game->getActiveScene());

	if (this->audioSystem != NULL)
		this->audioSystem->process(this->game->getActiveScene());

	if (this->hapticSystem != NULL)
		this->hapticSystem->process(this->game->getActiveScene(), this->os->hid());
}

void Lilliputian::Engine::sleep()
{
	float targetFrameTime_ms = 1000.0 / this->configuration.targetFPS;
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

void Lilliputian::Engine::setGameTitle(const char* gameTitle)
{
	this->configuration.gameTitle = gameTitle;
}

void Lilliputian::Engine::setEntryScene(const char* entryScenePath)
{
	this->configuration.entryScenePath = entryScenePath;
}

void Lilliputian::Engine::setWindowDimensions(uint16_t width, uint16_t height)
{
	if (height > 0)
		this->configuration.windowHeight = height;
	if (width > 0)
		this->configuration.windowWidth = width;
}

void Lilliputian::Engine::setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	if (msPerComputeUpdate > (1.0 / (this->configuration.targetFPS * 2.0)))
		this->configuration.msPerComputeUpdate = (1.0 / (this->configuration.targetFPS * 2.0));
	else
		this->configuration.msPerComputeUpdate = msPerComputeUpdate;
}

void Lilliputian::Engine::setTargetFPS(uint32_t targetFPS)
{
	if (targetFPS > 0)
		this->configuration.targetFPS = targetFPS;
	else
		this->configuration.targetFPS = 1;
}

void Lilliputian::Engine::setFullscreenStart(bool value)
{
	this->configuration.isStartingFullscreen = value;
}

void Lilliputian::Engine::addScriptCompilerFunction(ScriptCompilerCallback scriptCompilerCallback)
{
	this->scriptCompilerCallbacks.push_back(scriptCompilerCallback);
}