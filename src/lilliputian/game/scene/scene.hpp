#pragma once

#include "../../utilities/collections/vector.hpp"
#include "../../utilities/collections/stack.hpp"
#include "../../utilities/collections/queue.hpp"
#include "../../utilities/collections/map.hpp"
#include "assets/assets.hpp"
#include "scene_layer_2d.hpp"

namespace Lilliputian
{
	class Scene
	{
	public:
		void incrementSceneLayer2D();

		void addEntity2D(Entity2D entity2D);
		void addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D);
		void addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D);
		void addRectangularCollider2D(EntityID entityID, RectangularCollider2D rectangularCollider2D);
		void addCamera2D(EntityID entityID, Camera2D camera2D);
		void addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation);
		void addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D);
		void addAnimatedSprite2D(EntityID entityID, AnimatedSprite animatedSprite2D);

		EntityID getLastEntityID();

		void executeFrameLogic();
	private:
		uint64_t entityIDCount = 0;
		Vector<Stack<SceneLayer2D>> sceneLayer2DStacks;
		Assets assets;

		Stack<SceneLayer2D>* getLastSceneLayer2DStack();
	};
}