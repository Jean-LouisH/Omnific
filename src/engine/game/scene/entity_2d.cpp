#include "entity_2d.hpp"
#include "utilities/constants.hpp"

void Lilliputian::Entity2D::addParentEntity(EntityID parentEntityID)
{
	this->parentID = parentEntityID;
}

void Lilliputian::Entity2D::addChildEntity(EntityID childEntityID)
{
	this->childIDs.push_back(childEntityID);
}

void Lilliputian::Entity2D::addComponent(ComponentVariant::Type type, ComponentID ID)
{
	this->components.emplace(type, ID);
}

void Lilliputian::Entity2D::addScript(String scriptName)
{
	this->scriptNames.push_back(scriptName);
}
