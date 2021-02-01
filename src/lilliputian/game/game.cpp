#include "game.hpp"

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
	this->activeSceneStack.emplace(Scene());
	this->sceneSerializer = NULL;
}

Lilliputian::EngineConfiguration Lilliputian::Game::initialize()
{
	String assetsDirectory = this->os->fileAccess().getExecutableName() + "_Assets/";
	EngineConfiguration configuration;

	if (this->os->fileAccess().exists(assetsDirectory))
		configuration = this->initializeGame(assetsDirectory);
	else
		configuration = this->initializeEditor();

	return configuration;
}

Lilliputian::EngineConfiguration Lilliputian::Game::initializeEditor()
{
	Scene editorScene;
	EngineConfiguration configuration;

	this->activeSceneStack.emplace(editorScene);

	return configuration;
}

Lilliputian::EngineConfiguration Lilliputian::Game::initializeGame(String assetsDirectory)
{
	Scene entryScene;
	EngineConfiguration configuration;

	this->sceneSerializer = new SceneSerializer(assetsDirectory);
	this->activeSceneStack.emplace(entryScene);

	return configuration;
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