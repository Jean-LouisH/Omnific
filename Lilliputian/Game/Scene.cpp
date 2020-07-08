#include "Scene.hpp"

Lilliputian::Scene::Scene()
{
	this->entityIDCount = 0;
	this->audioListenerIDCount = 0;
	this->audioSourceIDCount = 0;
	this->boxColliderIDCount = 0;
	this->cameraIDCount = 0;
	this->propertyAnimationIDCount = 0;
	this->rigidBodyIDCount = 0;
	this->spriteIDCount = 0;
}

Lilliputian::Audio Lilliputian::Scene::loadAudio(const char* filepath)
{
	if (!this->audios.count(filepath))
	{
		this->audios.emplace(filepath, Audio(filepath));
		return this->audios.at(filepath);
	}
	else
	{
		return this->audios.at(filepath);
	}
}

Lilliputian::Font Lilliputian::Scene::loadFont(const char* filepath)
{
	if (!this->fonts.count(filepath))
	{
		this->fonts.emplace(filepath, Font(filepath));
		return this->fonts.at(filepath);
	}
	else
	{
		return this->fonts.at(filepath);
	}
}

Lilliputian::Texture Lilliputian::Scene::loadTexture(const char* filepath)
{
	if (!this->textures.count(filepath))
	{
		this->textures.emplace(filepath, Texture(filepath));
		return this->textures.at(filepath);
	}
	else
	{
		return this->textures.at(filepath);
	}
}

void Lilliputian::Scene::incrementSubscene2D()
{
	Subscene2D subscene2D;
	Stack<Subscene2D> subscene2Ds;

	subscene2Ds.push(subscene2D);
	this->subscene2Ds.push_back(subscene2Ds);
}

Lilliputian::Entity2D Lilliputian::Scene::newEntity2D()
{
	this->entityIDCount++;
	return Entity2D(this->entityIDCount);
}

Lilliputian::AudioListener2D Lilliputian::Scene::attachAudioListener2D(EntityID entityID)
{
	return AudioListener2D(entityID);
}

Lilliputian::AudioSource2D Lilliputian::Scene::attachAudioSource2D(EntityID entityID)
{
	return AudioSource2D(entityID);
}

Lilliputian::BoxCollider2D Lilliputian::Scene::attachBoxCollider2D(EntityID entityID)
{
	return BoxCollider2D(entityID);
}

Lilliputian::Camera2D Lilliputian::Scene::attachCamera2D(EntityID entityID)
{
	return Camera2D(entityID);
}

Lilliputian::PropertyAnimation Lilliputian::Scene::attachPropertyAnimation(EntityID entityID)
{
	return PropertyAnimation(entityID);
}

Lilliputian::RigidBody2D Lilliputian::Scene::attachRigidBody2D(EntityID entityID)
{
	return RigidBody2D(entityID);
}

Lilliputian::Sprite2D Lilliputian::Scene::attachSprite2D(EntityID entityID)
{
	return Sprite2D(entityID);
}

void Lilliputian::Scene::addEntity2D(Entity2D entity2D)
{
	this->subscene2Ds.at(this->subscene2Ds.size() - 1).top().addEntity2D(entity2D);
}

void Lilliputian::Scene::executeFrameLogic()
{
	for (int i = 0; i < this->subscene2Ds.size(); i++)
		this->subscene2Ds.at(i).top().executeFrameLogic();
}