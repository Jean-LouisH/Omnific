#include "Game.hpp"

Lilliputian::Game::Game()
{
	this->editor = new EditorAPI(&this->entryScene, &this->scenes);
}

void Lilliputian::Game::initialize()
{
	this->activeScenes.push(this->entryScene);
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

Lilliputian::EditorAPI* Lilliputian::Game::getEditor()
{
	return this->editor;
}

Lilliputian::Scene* Lilliputian::Game::getActiveScene()
{
	return &this->activeScenes.top();
}