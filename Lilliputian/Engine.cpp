#include "Engine.hpp"

Lilliputian::Engine::Engine(
	int argc, 
	char* argv[], 
	const char* gameTitle, 
	uint16_t windowWidth, 
	uint16_t windowHeight, 
	const char* entrySceneName)
{
	this->profiler = new Profiler();
	this->threadPool = new ThreadPool();
	this->fileSystem = new FileSystem(argv[0]);
	this->configuration.msPerComputeUpdate = 8;
	this->configuration.targetFPS = 60;
	this->configuration.windowHeight = 640;
	this->configuration.windowWidth = 480;
	this->configuration.isStartingFullscreen = false;

	this->setGameTitle(gameTitle);
	this->setWindowDimensions(windowWidth, windowHeight);
	this->setEntryScene(entrySceneName);
}

Lilliputian::Engine::~Engine()
{

}

void Lilliputian::Engine::run()
{
	do
	{
		this->initialize();
		this->game->initialize(this->fileSystem->getExecutableName() + "_Assets/", this->configuration.entryScenePath);

		this->state.setRunningApplicationWindowed();

		while (this->state.isRunning())
		{
			this->profiler->process.setStart();
			this->input();
			this->logic();
			this->compute();
			this->output();
			this->profiler->process.setEnd();
			this->sleep();
			this->profiler->frameCount++;
		}

		this->game->deinitialize();
		this->shutdown();
	} while (this->state.isRestarting());
}

void Lilliputian::Engine::initialize()
{
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
		this->osWindow = new OSWindow(
			this->configuration.gameTitle.c_str(),
			this->configuration.windowHeight,
			this->configuration.windowWidth,
			this->configuration.isStartingFullscreen);

		this->game = new Game(this->osWindow, this->fileSystem, this->profiler);

		for (int i = 0; i < this->scriptCompilerCallbacks.size(); i++)
		{
			if (this->scriptCompilerCallbacks.at(i) != nullptr)
				this->scriptCompilerCallbacks.at(i)(this->game->getScriptRegistry());
		}

		this->aiEngine = new AIEngine();
		this->animationEngine = new AnimationEngine();
		this->audioEngine = new AudioEngine();
		this->hapticEngine = new HapticEngine();
		this->physicsEngine = new PhysicsEngine();
		this->renderingEngine = new RenderingEngine(this->osWindow->getSDLWindow());
	}
}

void Lilliputian::Engine::input()
{
	this->osWindow->detectGameControllers();
	this->osWindow->pollInputEvents();
	if (this->osWindow->hasRequestedShutdown())
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
	if (this->renderingEngine != NULL)
		this->renderingEngine->process(this->game->getActiveScene());

	if (this->audioEngine != NULL)
		this->audioEngine->process(this->game->getActiveScene());

	if (this->hapticEngine != NULL)
		this->hapticEngine->process(this->game->getActiveScene(), this->osWindow->getHaptics());
}

void Lilliputian::Engine::sleep()
{
	float targetFrameTime_ms = 1000.0 / this->configuration.targetFPS;
	float ProcessTime_ms = this->profiler->process.getDelta_ns() / 1000.0;
	this->osWindow->sleep(targetFrameTime_ms - ProcessTime_ms);
}

void Lilliputian::Engine::shutdown()
{
	delete this->osWindow;
	delete this->game;
	delete this->aiEngine;
	delete this->animationEngine;
	delete this->audioEngine;
	delete this->hapticEngine;
	delete this->physicsEngine;
	delete this->renderingEngine;
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