#include "SceneLayer2D.hpp"

void Lilliputian::SceneLayer2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.emplace(entity2D.id, entity2D);
}

void Lilliputian::SceneLayer2D::addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D)
{
	audioListener2D.entityID = entityID;
	this->audioListeners2D.push_back(audioListener2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_AUDIO_LISTENER_2D, this->audioListeners2D.size() - 1);
}

void Lilliputian::SceneLayer2D::addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D)
{
	streamedAudioSource2D.entityID = entityID;
	this->streamedAudioSources2D.push_back(streamedAudioSource2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D, this->streamedAudioSources2D.size() - 1);
}

void Lilliputian::SceneLayer2D::addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D)
{
	boxCollider2D.entityID = entityID;
	this->boxColliders2D.push_back(boxCollider2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_BOX_COLLIDER_2D, this->boxColliders2D.size() - 1);
}

void Lilliputian::SceneLayer2D::addCamera2D(EntityID entityID, Camera2D camera2D)
{
	camera2D.entityID = entityID;
	this->cameras2D.push_back(camera2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_CAMERA_2D, this->cameras2D.size() - 1);
}

void Lilliputian::SceneLayer2D::addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation)
{
	propertyAnimation.entityID = entityID;
	this->propertyAnimations.push_back(propertyAnimation);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_PROPERTY_ANIMATION, this->propertyAnimations.size() - 1);
}

void Lilliputian::SceneLayer2D::addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D)
{
	rigidBody2D.entityID = entityID;
	this->rigidBodies2D.push_back(rigidBody2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_RIGID_BODY_2D, this->rigidBodies2D.size() - 1);
}

void Lilliputian::SceneLayer2D::addAnimatedSprite2D(EntityID entityID, AnimatedSprite animatedSprite2D)
{
	animatedSprite2D.entityID = entityID;
	this->animatedSprites.push_back(animatedSprite2D);
	this->entities2D.at(entityID).addComponent(COMPONENT_TYPE_ANIMATED_SPRITE_2D, this->animatedSprites.size() - 1);
}

void Lilliputian::SceneLayer2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}