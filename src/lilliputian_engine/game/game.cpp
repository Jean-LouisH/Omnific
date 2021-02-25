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
}

void Lilliputian::Game::initialize()
{
	BootLoader bootLoader;
	Scene entryScene;
	String assetsDirectory = "data/";
	String bootFilename = "boot.yml";
	String bootFilepath = assetsDirectory + bootFilename;

	if (this->os->fileAccess().exists(bootFilepath))
	{
		this->_configuration = bootLoader.loadFromFile(bootFilepath);
		this->sceneSerializer = new SceneSerializer(assetsDirectory);
		entryScene = this->sceneSerializer->loadFromFile(_configuration->entryScenePath);
		this->activeSceneStack.emplace(entryScene);
	}
	else
	{
		this->_configuration = new BootConfiguration();
		this->_configuration->windowHeight = 640;
		this->_configuration->windowWidth = 480;
		this->_configuration->isResizable = false;
		this->_configuration->isStartingFullscreen = false;
		this->_configuration->isStartingMaximized = false;
	}
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
	delete this->_configuration;
	delete this->scriptingAPIs;
	delete this->scriptRegistry;
	delete this->sceneSerializer;
}

Lilliputian::ScriptRegistry& Lilliputian::Game::getScriptRegistry()
{
	return *this->scriptRegistry;
}

Lilliputian::Scene& Lilliputian::Game::getActiveScene()
{
	return this->activeSceneStack.top();
}

Lilliputian::BootConfiguration& Lilliputian::Game::configuration()
{
	return *this->_configuration;
}