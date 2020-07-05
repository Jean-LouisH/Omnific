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

void Lilliputian::Entity2D::addBoxCollider(ComponentID boxCollider)
{
	this->components.emplace(COMPONENT_BOX_COLLIDER_2D, boxCollider);
}

void Lilliputian::Entity2D::addCamera(ComponentID camera)
{
	this->components.emplace(COMPONENT_CAMERA_2D, camera);
}

void Lilliputian::Entity2D::addRigidBody(ComponentID rigidBody)
{
	this->components.emplace(COMPONENT_RIGID_BODY_2D, rigidBody);
}

void Lilliputian::Entity2D::addSprite(ComponentID sprite)
{
	this->components.emplace(COMPONENT_SPRITE_2D, sprite);
}

void Lilliputian::Entity2D::addScript(String scriptName)
{
	this->scriptNames.push_back(scriptName);
}

Lilliputian::Map<Lilliputian::ComponentType, Lilliputian::ComponentID> Lilliputian::Entity2D::getComponents()
{
	return this->components;
}