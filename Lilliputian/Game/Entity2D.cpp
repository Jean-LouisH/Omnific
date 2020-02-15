#include "Entity2D.hpp"

Lilliputian::Entity2D::Entity2D()
{

}

void Lilliputian::Entity2D::addParentEntity(Entity2D entity)
{
	this->parentEntity = entity.entityID;
}

void Lilliputian::Entity2D::addChildEntity(Entity2D entity)
{
	this->childrenEntity.push_back(entity.entityID);
}

void Lilliputian::Entity2D::addScript(Script script)
{
	this->scripts.push_back(script);
}