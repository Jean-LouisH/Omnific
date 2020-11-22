#include "Game.hpp"

Lilliputian::Game::Game(
	OS* os,
	Profiler* profiler)
{
	this->os = os;
	this->profiler = profiler;
	this->scriptingAPIs = new ScriptingAPIs();
	this->scriptRegistry = new ScriptRegistry(
		this->scriptingAPIs,
		&this->scripts);
	this->editor = new Editor(os);
	this->activeSceneStack.emplace(Scene());
	this->sceneLoader = NULL;
}

void Lilliputian::Game::initialize()
{
	String assetsDirectory = this->os->fileAccess().getExecutableName() + "_Assets/";

	if (this->os->fileAccess().exists(assetsDirectory))
		this->initializeGame(assetsDirectory);
	else
		this->initializeEditor();
}

void Lilliputian::Game::initializeEditor()
{
	Scene editorScene;
	this->activeSceneStack.emplace(editorScene);
}

void Lilliputian::Game::initializeGame(String assetsDirectory)
{
	this->sceneLoader = new SceneLoader(assetsDirectory);
	Scene entryScene;
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

Lilliputian::Scene& Lilliputian::Game::getActiveScene()
{
	return this->activeSceneStack.top();
}