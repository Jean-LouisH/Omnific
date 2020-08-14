#pragma once

#include "Entity2D.hpp"
#include "AnimatedSprite2D.hpp"
#include "AudioListener2D.hpp"
#include "AudioSource2D.hpp"
#include "BoxCollider2D.hpp"
#include "Camera2D.hpp"
#include "PropertyAnimation.hpp"
#include "RigidBody2D.hpp"
#include "../Utilities/Rectangle.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Map.hpp"

namespace Lilliputian
{
	class Subscene2D
	{
	private:
		Map<EntityID, Entity2D> entities2D;
		Vector<AudioListener2D> audioListeners2D;
		Vector<StreamedAudioSource2D> audioSources2D;
		Vector<BoxCollider2D> boxColliders2D;
		Vector<Camera2D> cameras2D;
		Vector<PropertyAnimation> propertyAnimations;
		Vector<RigidBody2D> rigidBodies2D;
		Vector<AnimatedSprite2D> animatedSprites2D;
		Rectangle windowDimensions;
	public:
		Subscene2D()
		{
			this->windowDimensions.width = 0;
			this->windowDimensions.height = 0;
		}

		void addEntity2D(Entity2D entity2D);
		void addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D);
		void addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D);
		void addBoxCollider2D(EntityID entityID, BoxCollider2D boxCollider2D);
		void addCamera2D(EntityID entityID, Camera2D camera2D);
		void addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation);
		void addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D);
		void addAnimatedSprite2D(EntityID entityID, AnimatedSprite2D sprite2D);

		void executeFrameLogic();
	};
}