#include "Editor.hpp"

Lilliputian::Editor::Editor(Scene* mainScene, std::vector<Scene>* scenes)
{
	this->scriptingAPI = new ScriptingAPI();
	this->mainScene = mainScene;
	this->scenes = scenes;
	this->entityIDCount = 0;
}

Lilliputian::Scene Lilliputian::Editor::createNewScene()
{
	return Scene();
}

Lilliputian::Script Lilliputian::Editor::createNewScript()
{
	return Script(this->scriptingAPI);
}

Lilliputian::Entity2D Lilliputian::Editor::createNewEntity2D()
{
	Entity2D newEntity2D = Entity2D((EntityID)this->entityIDCount);
	this->entityIDCount++;
	return newEntity2D;
}

void Lilliputian::Editor::setMainScene(Scene scene)
{
	*this->mainScene = scene;
}

void Lilliputian::Editor::addScene(Scene scene)
{
	this->scenes->push_back(scene);
}