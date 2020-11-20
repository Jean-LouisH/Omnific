#pragma once

#include "Entity2D.hpp"
#include "Components/AnimatedSprite.hpp"
#include "Components/AudioListener2D.hpp"
#include "Components/StreamedAudioSource2D.hpp"
#include "Components/RectangularCollider2D.hpp"
#include "Components/Camera2D.hpp"
#include "Components/PropertyAnimation.hpp"
#include "Components/RigidBody2D.hpp"
#include "../../Utilities/Rectangle.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "../../Utilities/Collections/Map.hpp"
#include "../../Utilities/Aliases.hpp"

namespace Lilliputian
{
	class SceneLayer2D
	{
	private:
		Map<EntityID, Entity2D> entities2D;
		Vector<AudioListener2D> audioListeners2D;
		Vector<StreamedAudioSource2D> streamedAudioSources2D;
		Vector<RectangularCollider2D> boxColliders2D;
		Vector<Camera2D> cameras2D;
		Vector<PropertyAnimation> propertyAnimations;
		Vector<RigidBody2D> rigidBodies2D;
		Vector<AnimatedSprite> animatedSprites;
		Rectangle windowDimensions;
		ComponentID currentCamera = 0;
	public:
		SceneLayer2D()
		{
			this->windowDimensions.width = 0;
			this->windowDimensions.height = 0;
		}

		void addEntity2D(Entity2D entity2D);
		void addAudioListener2D(EntityID entityID, AudioListener2D audioListener2D);
		void addStreamedAudioSource2D(EntityID entityID, StreamedAudioSource2D streamedAudioSource2D);
		void addRectangularCollider2D(EntityID entityID, RectangularCollider2D rectangularCollider2D);
		void addCamera2D(EntityID entityID, Camera2D camera2D);
		void addPropertyAnimation(EntityID entityID, PropertyAnimation propertyAnimation);
		void addRigidBody2D(EntityID entityID, RigidBody2D rigidBody2D);
		void addAnimatedSprite2D(EntityID entityID, AnimatedSprite animatedSprite);

		void executeFrameLogic();
	};
}