#include "Engine.hpp"

Lilliputian::Engine::Engine(const char* gameTitle, uint16_t windowWidth, uint16_t windowHeight, const char* entrySceneName)
{
	this->game = NULL;
	this->aiEngine = NULL;
	this->animationEngine = NULL;
	this->audioEngine = NULL;
	this->hapticEngine = NULL;
	this->physicsEngine = NULL;
	this->renderingEngine = NULL;
	this->osWindow = NULL;
	this->platform = NULL;
	this->profiler = NULL;
	this->frameCount = 0;
	this->FPS = 0;
	this->msPerComputeUpdate = 8;
	this->targetFPS = 60;
	this->windowHeight = 640;
	this->windowWidth = 480;
	this->isStartingFullscreen = false;

	this->setGameTitle(gameTitle);
	this->setWindowDimensions(windowWidth, windowHeight);
	this->setEntryScene(entrySceneName);
}

Lilliputian::Engine::~Engine()
{

}

void Lilliputian::Engine::runGame()
{
	do
	{
		this->initialize();
		this->game->initialize();

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
			this->frameCount++;
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
		this->game = new Game();

		for (int i = 0; i < this->scriptCompilerCallbacks.size(); i++)
		{
			if (this->scriptCompilerCallbacks.at(i) != nullptr)
				this->scriptCompilerCallbacks.at(i)(this->game->getScriptRegistry());
		}

		this->osWindow = new OSWindow(
			this->gameTitle.c_str(),
			this->windowHeight,
			this->windowWidth,
			this->isStartingFullscreen);

		this->profiler = new Profiler();
		this->audioEngine = new AudioEngine();
		this->renderingEngine = new RenderingEngine(this->osWindow->getSDLWindow());
		this->hapticEngine = new HapticEngine();
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
	double targetFrameTime_ms = 1000.0 / this->targetFPS;
	double ProcessTime_ms = this->profiler->process.getDelta_ns() / 1000.0;
	this->osWindow->sleep(targetFrameTime_ms - ProcessTime_ms);
}

void Lilliputian::Engine::shutdown()
{
	delete this->osWindow;
	delete this->profiler;
	delete this->renderingEngine;
	delete this->game;
}

void Lilliputian::Engine::setGameTitle(const char* gameTitle)
{
	this->gameTitle = gameTitle;
}

void Lilliputian::Engine::setEntryScene(const char* entrySceneName)
{
	this->entrySceneName = entrySceneName;
}

void Lilliputian::Engine::setWindowDimensions(uint16_t width, uint16_t height)
{
	if (height > 0)
		this->windowHeight = height;
	if (width > 0)
		this->windowWidth = width;
}

void Lilliputian::Engine::setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	if (msPerComputeUpdate > (1.0 / (this->targetFPS * 2.0)))
		this->msPerComputeUpdate = (1.0 / (this->targetFPS * 2.0));
	else
		this->msPerComputeUpdate = msPerComputeUpdate;
}

void Lilliputian::Engine::setTargetFPS(uint32_t targetFPS)
{
	if (targetFPS > 0)
		this->targetFPS = targetFPS;
	else
		this->targetFPS = 1;
}

void Lilliputian::Engine::setFullscreenStart(bool value)
{
	this->isStartingFullscreen = value;
}

void Lilliputian::Engine::addScriptCompiler(ScriptCompilerCallback scriptCompilerCallback)
{
	this->scriptCompilerCallbacks.push_back(scriptCompilerCallback);
}