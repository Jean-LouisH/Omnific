#pragma once

#include "../../Utilities/Collections/Vector.hpp"
#include "../../Utilities/Collections/Stack.hpp"
#include "../../Utilities/Collections/Queue.hpp"
#include "../../Utilities/Collections/Map.hpp"
#include "Assets/Assets.hpp"
#include "SceneLayer2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		uint64_t entityIDCount = 0;
		Vector<Stack<SceneLayer2D>> sceneLayer2DStacks;
		Assets assets;

		Stack<SceneLayer2D>* getLastSceneLayer2DStack();
	public:
		void incrementSceneLayer2D();

		void addEntity2D(Entity2D entity2D);
		void addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D);
		void addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D);
		void addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D);
		void addCamera2D(EntityID entityID, Camera2D camera2D);
		void addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation);
		void addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D);
		void addAnimatedSprite2D(EntityID entityID, AnimatedSprite animatedSprite2D);

		EntityID getLastEntityID();

		void executeFrameLogic();
	};
}