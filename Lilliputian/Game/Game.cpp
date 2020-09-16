#include "Game.hpp"

Lilliputian::Game::Game(
	OSWindow* osWindow,
	FileSystem* fileSystem,
	Profiler* profiler)
{
	this->osWindow = osWindow;
	this->fileSystem = fileSystem;
	this->profiler = profiler;
	this->scriptingAPIs = new ScriptingAPIs();
	this->scriptRegistry = new ScriptRegistry(
		this->scriptingAPIs,
		&this->scripts);
	this->activeSceneStack.emplace(Scene());
	this->sceneLoader = NULL;
}

void Lilliputian::Game::initialize(String assetsDirectory, String entryScenePath)
{
	this->sceneLoader = new SceneLoader(assetsDirectory);
	Scene entryScene = this->sceneLoader->loadScene(entryScenePath.c_str());
	this->activeSceneStack.emplace(entryScene);
}

void Lilliputian::Game::executeStartLogic()
{

}

void Lilliputian::Game::executeInputLogic()
{

}

void Lilliputian::Game::executeFrameLogic()
{
	if (this->activeSceneStack.size() > 0)
		this->activeSceneStack.top().executeFrameLogic();
}

void Lilliputian::Game::executeComputeLogic(unsigned int computeTimeDelta_ms)
{

}

void Lilliputian::Game::executeLateLogic()
{

}

void Lilliputian::Game::executeFinalLogic()
{

}

void Lilliputian::Game::deinitialize()
{

}

Lilliputian::ScriptRegistry& Lilliputian::Game::getScriptRegistry()
{
	return *this->scriptRegistry;
}

Lilliputian::Scene* Lilliputian::Game::getActiveScene()
{
	return &this->activeSceneStack.top();
}