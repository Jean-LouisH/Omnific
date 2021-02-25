#include "game.hpp"
#include "boot_loader.hpp"

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
	EngineConfiguration configuration;
	BootLoader bootLoader;
	Scene entryScene;
	String assetsDirectory = "data/";
	String bootFilepath = "boot.yml";

	if (this->os->fileAccess().exists(assetsDirectory + bootFilepath))
	{
		configuration = bootLoader.loadFromFile(assetsDirectory + bootFilepath);
		this->sceneSerializer = new SceneSerializer(assetsDirectory);
		entryScene = this->sceneSerializer->loadFromFile(configuration.entryScenePath);
		this->activeSceneStack.emplace(entryScene);
	}

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