#include "EditorAPI.hpp"

Lilliputian::EditorAPI::EditorAPI(uint8_t entrySceneIndex, std::vector<Scene>* scenes)
{
	this->scriptingAPI = new ScriptingAPI();
	this->entrySceneIndex = entrySceneIndex;
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

void Lilliputian::EditorAPI::setEntryScene(Scene scene)
{
	this->scenes->push_back(scene);
	this->entrySceneIndex = this->scenes->size();
}

void Lilliputian::EditorAPI::addScene(Scene scene)
{
	this->scenes->push_back(scene);
}