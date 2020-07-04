#include "Game.hpp"

Lilliputian::Game::Game()
{
	this->gameAPI = new GameAPI();
	this->inputAPI = new InputAPI();
	this->scriptRegistry = new ScriptRegistry(new ScriptingAPI(this->gameAPI, this->inputAPI), &this->scripts);
	this->activeScenes.emplace(Scene());
	this->sceneLoader = NULL;
}

void Lilliputian::Game::initialize(String assetsDirectory, String entryScenePath)
{
	this->sceneLoader = new SceneLoader(assetsDirectory);
	Scene entryScene = this->sceneLoader->loadScene(entryScenePath.c_str());
	this->activeScenes.emplace(entryScene);
}

void Lilliputian::Game::executeStartLogic()
{

}

void Lilliputian::Game::executeInputLogic()
{

}

void Lilliputian::Game::executeFrameLogic()
{
	if (this->activeScenes.size() > 0)
		this->activeScenes.top().executeFrameLogic();
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

Lilliputian::InputAPI* Lilliputian::Game::getInputAPI()
{
	return this->inputAPI;
}

Lilliputian::ScriptRegistry* Lilliputian::Game::getScriptRegistry()
{
	return this->scriptRegistry;
}

Lilliputian::Scene* Lilliputian::Game::getActiveScene()
{
	return &this->activeScenes.top();
}