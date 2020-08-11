#pragma once

#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Queue.hpp"
#include "../Utilities/Collections/Map.hpp"
#include "Assets.hpp"
#include "Subscene2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		uint64_t entityIDCount = 0;
		Vector<Stack<Subscene2D>> subscene2Ds;
		Assets assets;

		Stack<Subscene2D>* getLastSubscene2D();
	public:
		void incrementSubscene2D();

		void addEntity2D(Entity2D entity2D);
		void addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D);
		void addAudioSource2D(EntityID entityID, AudioSource2D audioSource2D);
		void addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D);
		void addCamera2D(EntityID entityID, Camera2D camera2D);
		void addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation);
		void addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D);
		void addSprite2D(EntityID entityID, Sprite2D sprite2D);

		EntityID getLastEntityID();

		void executeFrameLogic();
	};
}