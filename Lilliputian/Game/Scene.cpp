#include "Scene.hpp"

Lilliputian::Stack<Lilliputian::SceneLayer2D>* Lilliputian::Scene::getLastSceneLayer2D()
{
	return &(this->sceneLayer2Ds.at(this->sceneLayer2Ds.size() - 1));
}

void Lilliputian::Scene::incrementSceneLayer2D()
{
	SceneLayer2D sceneLayer2D;
	Stack<SceneLayer2D> sceneLayer2Ds;

	sceneLayer2Ds.push(sceneLayer2D);
	this->sceneLayer2Ds.push_back(sceneLayer2Ds);
}

void Lilliputian::Scene::addEntity2D(Entity2D entity2D)
{
	this->entityIDCount++;
	entity2D.id = this->entityIDCount;
	this->getLastSceneLayer2D()->top().addEntity2D(entity2D);
}

void Lilliputian::Scene::addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D)
{
	this->getLastSceneLayer2D()->top().addAudioListener2D(entityID, audioListener2D);
}

void Lilliputian::Scene::addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D)
{
	this->getLastSceneLayer2D()->top().addStreamedAudioSource2D(entityID, streamedAudioSource2D);
}

void Lilliputian::Scene::addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D)
{
	this->getLastSceneLayer2D()->top().addBoxCollider2D(entityID, boxCollider2D);
}

void Lilliputian::Scene::addCamera2D(EntityID entityID, Camera2D camera2D)
{
	this->getLastSceneLayer2D()->top().addCamera2D(entityID, camera2D);
}

void Lilliputian::Scene::addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation)
{
	this->getLastSceneLayer2D()->top().addPropertyAnimation(entityID, propertyAnimation);
}

void Lilliputian::Scene::addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D)
{
	this->getLastSceneLayer2D()->top().addRigidBody2D(entityID, rigidBody2D);
}

void Lilliputian::Scene::addAnimatedSprite2D(EntityID entityID, AnimatedSprite2D sprite2D)
{
	this->getLastSceneLayer2D()->top().addAnimatedSprite2D(entityID, sprite2D);
}

Lilliputian::EntityID Lilliputian::Scene::getLastEntityID()
{
	return this->entityIDCount;
}

void Lilliputian::Scene::executeFrameLogic()
{
	for (int i = 0; i < this->sceneLayer2Ds.size(); i++)
		this->sceneLayer2Ds.at(i).top().executeFrameLogic();
}