#include "Entity2D.hpp"

Lilliputian::Entity2D::Entity2D(EntityID id)
{
	this->parentEntity = nullptr;
	this->id = id;
}

void Lilliputian::Entity2D::addParentEntity(Entity2D entity)
{
	*this->parentEntity = entity;
}

void Lilliputian::Entity2D::addChildEntity(Entity2D entity)
{
	this->childrenEntities.push_back(entity);
}

void Lilliputian::Entity2D::addScript(Script script)
{
	this->scripts.push_back(script);
}

void Lilliputian::Entity2D::executeFrameLogic()
{
	for (int i = 0; i < this->scripts.size(); i++)
		this->scripts.at(i).executeFrameLogic();
}