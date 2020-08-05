#include "Entity2D.hpp"
#include "../Utilities/Constants.hpp"

void Lilliputian::Entity2D::addParentEntity(EntityID parentEntityID)
{
	this->parentEntity = parentEntityID;
}

void Lilliputian::Entity2D::addChildEntity(EntityID childEntityID)
{
	this->childrenEntities.push_back(childEntityID);
}

void Lilliputian::Entity2D::addAudioListener(ComponentID audioListenerID)
{
	this->components.emplace(COMPONENT_AUDIO_LISTENER_2D, audioListenerID);
}

void Lilliputian::Entity2D::addAudioSource(ComponentID audioSourceID)
{
	this->components.emplace(COMPONENT_AUDIO_SOURCE_2D, audioSourceID);
}

void Lilliputian::Entity2D::addBoxCollider(ComponentID boxColliderID)
{
	this->components.emplace(COMPONENT_BOX_COLLIDER_2D, boxColliderID);
}

void Lilliputian::Entity2D::addCamera(ComponentID cameraID)
{
	this->components.emplace(COMPONENT_CAMERA_2D, cameraID);
}

void Lilliputian::Entity2D::addPropertyAnimation(ComponentID propertyAnimationID)
{
	this->components.emplace(COMPONENT_PROPERTY_ANIMATION, propertyAnimationID);
}

void Lilliputian::Entity2D::addRigidBody(ComponentID rigidBodyID)
{
	this->components.emplace(COMPONENT_RIGID_BODY_2D, rigidBodyID);
}

void Lilliputian::Entity2D::addSprite(ComponentID spriteID)
{
	this->components.emplace(COMPONENT_SPRITE_2D, spriteID);
}

void Lilliputian::Entity2D::addScript(String scriptName)
{
	this->scriptNames.push_back(scriptName);
}

Lilliputian::Map<Lilliputian::ComponentType, Lilliputian::ComponentID> Lilliputian::Entity2D::getComponents()
{
	return this->components;
}