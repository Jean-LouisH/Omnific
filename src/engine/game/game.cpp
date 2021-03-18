#include "game.hpp"
#include "boot_loader.hpp"
#include "utilities/constants.hpp"

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
}

void Lilliputian::Game::initialize()
{
	BootLoader bootLoader;
	SceneForest entryScene;
	String dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	String bootFilename = "boot.yml";
	String bootFilepath = dataDirectory + bootFilename;

	if (this->os->fileAccess().exists(bootFilepath))
	{
		this->_configuration = bootLoader.loadFromFile(bootFilepath);
		this->sceneSerializer = new SceneSerializer(dataDirectory);
		entryScene = this->sceneSerializer->loadFromFile(_configuration->entrySceneFilepath);
		this->activeSceneStack.emplace(entryScene);
	}
	else
	{
		this->_configuration = new BootConfiguration();
		this->_configuration->isLoaded = false;
	}
}

void Lilliputian::Game::executeOnStartMethods()
{

}

void Lilliputian::Game::executeOnInputMethods()
{

}

void Lilliputian::Game::executeOnFrameMethods()
{
	if (this->activeSceneStack.size() > 0)
		this->activeSceneStack.top().executeFrameLogic();
}

void Lilliputian::Game::executeOnComputeMethods()
{
	uint32_t msPerComputeUpdate = this->_configuration->msPerComputeUpdate;
}

void Lilliputian::Game::executeOnLateMethods()
{

}

void Lilliputian::Game::executeOnFinalMethods()
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

Lilliputian::SceneForest& Lilliputian::Game::getActiveScene()
{
	return this->activeSceneStack.top();
}

Lilliputian::BootConfiguration& Lilliputian::Game::configuration()
{
	return *this->_configuration;
}