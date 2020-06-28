#include "Game.hpp"

Lilliputian::Game::Game()
{
	this->entrySceneIndex = 0;
	this->editorAPI = new EditorAPI(&this->entrySceneIndex, &this->scenes);
	this->inputAPI = new InputAPI();
}

void Lilliputian::Game::initialize()
{
	this->activeScenes.push(this->scenes.at(this->entrySceneIndex));
}

void Lilliputian::Game::executeStartLogic()
{

}

void Lilliputian::Game::executeInputLogic()
{

}

void Lilliputian::Game::executeFrameLogic()
{
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

Lilliputian::EditorAPI* Lilliputian::Game::getEditorAPI()
{
	return this->editorAPI;
}

Lilliputian::InputAPI* Lilliputian::Game::getInputAPI()
{
	return this->inputAPI;
}

Lilliputian::Scene* Lilliputian::Game::getActiveScene()
{
	return &this->activeScenes.top();
}