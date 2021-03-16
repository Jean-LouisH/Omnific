#include "scene_tree_2d.hpp"

void Lilliputian::SceneTree2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.emplace(entity2D.ID, entity2D);
}

void Lilliputian::SceneTree2D::addTransform(EntityID entityID)
{
	Transform2D transform2D;
	this->transforms2D.push_back(transform2D);
	this->entities2D.at(entityID).setTransform(this->transforms2D.size() - 1);
}

void Lilliputian::SceneTree2D::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.entityID = entityID;
	this->componentVariants.push_back(componentVariant);
	this->entities2D.at(entityID).addComponent(componentVariant.type, this->componentVariants.size() - 1);
}

void Lilliputian::SceneTree2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}

Lilliputian::Vector<Lilliputian::ComponentVariant>& Lilliputian::SceneTree2D::getComponentVariants()
{
	return this->componentVariants;
}

Lilliputian::Transform2D& Lilliputian::SceneTree2D::getEntityTransform(EntityID entityID)
{
	return this->transforms2D.at(this->entities2D.at(entityID).transformID);
}