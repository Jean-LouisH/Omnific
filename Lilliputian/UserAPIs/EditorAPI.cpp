#include "EditorAPI.hpp"

Lilliputian::EditorAPI::EditorAPI(Scene* mainScene, std::vector<Scene>* scenes)
{
	this->scriptingAPI = new ScriptingAPI();
	this->mainScene = mainScene;
	this->scenes = scenes;
	this->entityIDCount = 0;
}

Lilliputian::Scene Lilliputian::EditorAPI::createNewScene()
{
	return Scene();
}

Lilliputian::Script Lilliputian::EditorAPI::createNewScript()
{
	return Script(this->scriptingAPI);
}

Lilliputian::Entity2D Lilliputian::EditorAPI::createNewEntity2D()
{
	Entity2D newEntity2D = Entity2D((EntityID)this->entityIDCount);
	this->entityIDCount++;
	return newEntity2D;
}

void Lilliputian::EditorAPI::setMainScene(Scene scene)
{
	*this->mainScene = scene;
}

void Lilliputian::EditorAPI::addScene(Scene scene)
{
	this->scenes->push_back(scene);
}