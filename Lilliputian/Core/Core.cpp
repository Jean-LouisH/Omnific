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

}

void Lilliputian::Core::compute()
{

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
	this->editor = new Editor();

	for (int i = 0; i < this->sceneDefinerCallbacks.size(); i++)
	{
		if (this->sceneDefinerCallbacks.at(i))
			this->sceneDefinerCallbacks.at(i)(this->editor);
	}
}

void Lilliputian::Core::run()
{
	this->initialize();

	this->shutdown();
}

void Lilliputian::Core::shutdown()
{
	delete this->editor;
}

void Lilliputian::Core::addSceneDefiner(SceneDefinerCallback sceneDefinerCallback)
{
	this->sceneDefinerCallbacks.push_back(sceneDefinerCallback);
}