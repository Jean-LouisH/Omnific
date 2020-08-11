#include "Scene.hpp"

Lilliputian::Stack<Lilliputian::Subscene2D>* Lilliputian::Scene::getLastSubscene2D()
{
	return &(this->subscene2Ds.at(this->subscene2Ds.size() - 1));
}

void Lilliputian::Scene::incrementSubscene2D()
{
	Subscene2D subscene2D;
	Stack<Subscene2D> subscene2Ds;

	subscene2Ds.push(subscene2D);
	this->subscene2Ds.push_back(subscene2Ds);
}

void Lilliputian::Scene::addEntity2D(Entity2D entity2D)
{
	this->entityIDCount++;
	entity2D.id = this->entityIDCount;
	this->getLastSubscene2D()->top().addEntity2D(entity2D);
}

void Lilliputian::Scene::addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D)
{
	this->getLastSubscene2D()->top().addAudioListener2D(entityID, audioListener2D);
}

void Lilliputian::Scene::addAudioSource2D(EntityID entityID, AudioSource2D audioSource2D)
{
	this->getLastSubscene2D()->top().addAudioSource2D(entityID, audioSource2D);
}

void Lilliputian::Scene::addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D)
{
	this->getLastSubscene2D()->top().addBoxCollider2D(entityID, boxCollider2D);
}

void Lilliputian::Scene::addCamera2D(EntityID entityID, Camera2D camera2D)
{
	this->getLastSubscene2D()->top().addCamera2D(entityID, camera2D);
}

void Lilliputian::Scene::addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation)
{
	this->getLastSubscene2D()->top().addPropertyAnimation(entityID, propertyAnimation);
}

void Lilliputian::Scene::addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D)
{
	this->getLastSubscene2D()->top().addRigidBody2D(entityID, rigidBody2D);
}

void Lilliputian::Scene::addAnimatedSprite2D(EntityID entityID, AnimatedSprite2D sprite2D)
{
	this->getLastSubscene2D()->top().addAnimatedSprite2D(entityID, sprite2D);
}

Lilliputian::EntityID Lilliputian::Scene::getLastEntityID()
{
	return this->entityIDCount;
}

void Lilliputian::Scene::executeFrameLogic()
{
	for (int i = 0; i < this->subscene2Ds.size(); i++)
		this->subscene2Ds.at(i).top().executeFrameLogic();
}