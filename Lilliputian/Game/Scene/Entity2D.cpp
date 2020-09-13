#include "Entity2D.hpp"
#include "../../Utilities/Constants.hpp"

void Lilliputian::Entity2D::addParentEntity(EntityID parentEntityID)
{
	this->parentEntity = parentEntityID;
}

void Lilliputian::Entity2D::addChildEntity(EntityID childEntityID)
{
	this->childrenEntities.push_back(childEntityID);
}

void Lilliputian::Entity2D::addComponent(ComponentType type, ComponentID ID)
{
	this->components.emplace(type, ID);
}

void Lilliputian::Entity2D::addScript(String scriptName)
{
	this->scriptNames.push_back(scriptName);
}

Lilliputian::Map<Lilliputian::ComponentType, Lilliputian::ComponentID> Lilliputian::Entity2D::getComponents()
{
	return this->components;
}