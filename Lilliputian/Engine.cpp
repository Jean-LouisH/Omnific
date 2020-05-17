#include "Engine.hpp"

Lilliputian::Engine::Engine()
{

}

Lilliputian::Engine::~Engine()
{

}

void Lilliputian::Engine::sleep()
{

}

void Lilliputian::Engine::benchmark()
{

}

void Lilliputian::Engine::input()
{

}

void Lilliputian::Engine::logic()
{
	this->game->executeStartLogic();
	this->game->executeInputLogic();
	this->game->executeFrameLogic();
}

void Lilliputian::Engine::compute()
{

	this->game->executeLateLogic();
	this->game->executeFinalLogic();
}

void Lilliputian::Engine::output()
{

}

void Lilliputian::Engine::initialize()
{
	this->game = new Game();

	for (int i = 0; i < this->sceneDefinerCallbacks.size(); i++)
	{
		if (this->sceneDefinerCallbacks.at(i) != nullptr)
			this->sceneDefinerCallbacks.at(i)(this->game->getEditor());
	}
}

void Lilliputian::Engine::shutdown()
{
	delete this->game;
}

void Lilliputian::Engine::run()
{
	this->initialize();
	this->game->initialize();

	//while()
	{
		this->input();
		this->logic();
		this->compute();
		this->output();
		this->sleep();
		this->benchmark();
	}

	this->game->deinitialize();
	this->shutdown();
}

void Lilliputian::Engine::addSceneDefiner(SceneDefinerCallback sceneDefinerCallback)
{
	this->sceneDefinerCallbacks.push_back(sceneDefinerCallback);
}