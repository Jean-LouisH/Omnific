#include "scene_forest.hpp"

Lilliputian::Stack<Lilliputian::SceneTree2D>* Lilliputian::SceneForest::getLastSceneTree2DStack()
{
	return &(this->sceneTree2DStacks.at(this->sceneTree2DStacks.size() - 1));
}

void Lilliputian::SceneForest::incrementSceneTree2D()
{
	Entity2D emptyEntity2D;
	SceneTree2D sceneTree2D;
	Stack<SceneTree2D> sceneTree2DStack;

	sceneTree2DStack.push(sceneTree2D);
	this->sceneTree2DStacks.push_back(sceneTree2DStack);
	this->addEntity2D(emptyEntity2D);
}

void Lilliputian::SceneForest::addEntity2D(Entity2D entity2D)
{
	entity2D.id = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2DStack()->top().addEntity2D(entity2D);
}

void Lilliputian::SceneForest::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	this->getLastSceneTree2DStack()->top().addComponent(entityID, componentVariant);
}

void Lilliputian::SceneForest::addComponentToLastEntity(ComponentVariant componentVariant)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->getLastSceneTree2DStack()->top().addComponent(lastEntityID, componentVariant);
}

Lilliputian::EntityID Lilliputian::SceneForest::getPreviousEntityID()
{
	return this->entityIDCount - 1;
}

void Lilliputian::SceneForest::executeFrameLogic()
{
	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
		this->sceneTree2DStacks.at(i).top().executeFrameLogic();
}