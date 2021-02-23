#pragma once

#include "entity_2d.hpp"
#include "components/animated_sprite.hpp"
#include "components/audio_listener_2d.hpp"
#include "components/streamed_audio_source_2d.hpp"
#include "components/rectangular_collider_2d.hpp"
#include "components/camera_2d.hpp"
#include "components/property_animation.hpp"
#include "components/rigid_body_2d.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/aliases.hpp"

namespace Lilliputian
{
	class SceneLayer2D
	{
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
	};
}