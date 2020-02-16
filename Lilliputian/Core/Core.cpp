#include "Core.hpp"

void Lilliputian::Core::sleep()
{

}

void Lilliputian::Core::benchmark()
{

}

void Lilliputian::Core::input()
{

}

void Lilliputian::Core::logic()
{
	this->game->executeStartLogic();
	this->game->executeInputLogic();
	this->game->executeFrameLogic();
}

void Lilliputian::Core::compute()
{

	this->game->executeLateLogic();
	this->game->executeFinalLogic();
}

void Lilliputian::Core::output()
{

}

Lilliputian::Core::Core()
{

}

Lilliputian::Core::~Core()
{

}

void Lilliputian::Core::initialize()
{
	this->game = new Game();

	for (int i = 0; i < this->sceneDefinerCallbacks.size(); i++)
	{
		if (this->sceneDefinerCallbacks.at(i) != nullptr)
			this->sceneDefinerCallbacks.at(i)(this->game->getEditor());
	}
}

void Lilliputian::Core::shutdown()
{
	delete this->game;
}

void Lilliputian::Core::run()
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

void Lilliputian::Core::addSceneDefiner(SceneDefinerCallback sceneDefinerCallback)
{
	this->sceneDefinerCallbacks.push_back(sceneDefinerCallback);
}