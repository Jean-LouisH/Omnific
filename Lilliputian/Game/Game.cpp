#include "Game.hpp"

Lilliputian::Game::Game()
{
	this->editor = new Editor(&this->entryScene, &this->scenes);
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

Lilliputian::Editor* Lilliputian::Game::getEditor()
{
	return this->editor;
}