#include "Subscene2D.hpp"

void Lilliputian::Subscene2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.emplace(entity2D.id, entity2D);
}

void Lilliputian::Subscene2D::addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D)
{
	audioListener2D.entityID = entityID;
	this->audioListeners2D.push_back(audioListener2D);
	this->entities2D.at(entityID).addAudioListener(this->audioListeners2D.size() - 1);
}

void Lilliputian::Subscene2D::addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D)
{
	streamedAudioSource2D.entityID = entityID;
	this->audioSources2D.push_back(streamedAudioSource2D);
	this->entities2D.at(entityID).addStreamedAudioSource(this->audioSources2D.size() - 1);
}

void Lilliputian::Subscene2D::addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D)
{
	boxCollider2D.entityID = entityID;
	this->boxColliders2D.push_back(boxCollider2D);
	this->entities2D.at(entityID).addBoxCollider(this->boxColliders2D.size() - 1);
}

void Lilliputian::Subscene2D::addCamera2D(EntityID entityID, Camera2D camera2D)
{
	camera2D.entityID = entityID;
	this->cameras2D.push_back(camera2D);
	this->entities2D.at(entityID).addCamera(this->cameras2D.size() - 1);
}

void Lilliputian::Subscene2D::addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation)
{
	propertyAnimation.entityID = entityID;
	this->propertyAnimations.push_back(propertyAnimation);
	this->entities2D.at(entityID).addPropertyAnimation(this->propertyAnimations.size() - 1);
}

void Lilliputian::Subscene2D::addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D)
{
	rigidBody2D.entityID = entityID;
	this->rigidBodies2D.push_back(rigidBody2D);
	this->entities2D.at(entityID).addRigidBody(this->rigidBodies2D.size() - 1);
}

void Lilliputian::Subscene2D::addAnimatedSprite2D(EntityID entityID, AnimatedSprite2D sprite2D)
{
	sprite2D.entityID = entityID;
	this->animatedSprites2D.push_back(sprite2D);
	this->entities2D.at(entityID).addAnimatedSprite(this->animatedSprites2D.size() - 1);
}

void Lilliputian::Subscene2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}