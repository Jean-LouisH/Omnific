#pragma once

#include "Script.hpp"
#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Transform2D.hpp"
#include "../Utilities/Constants.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	private:
		enum Components
		{
			COMPONENT_NONE,
			COMPONENT_AUDIO_LISTENER_2D,
			COMPONENT_AUDIO_SOURCE_2D,
			COMPONENT_BOX_COLLIDER_2D,
			COMPONENT_CAMERA_2D,
			COMPONENT_PROPERTY_ANIMATION,
			COMPONENT_RIGID_BODY_2D,
			COMPONENT_SPRITE_2D
		};

		Vector<String> scriptNames;
		Map<ComponentType, ComponentID> components;
	public:
		Transform2D transform2D;
		EntityID id = NO_ENTITY;
		EntityID parentEntity = NO_ENTITY;
		Vector<EntityID> childrenEntities;

		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addAudioListener(ComponentID audioListenerID);
		void addAudioSource(ComponentID audioSourceID);
		void addBoxCollider(ComponentID boxColliderID);
		void addCamera(ComponentID cameraID);
		void addPropertyAnimation(ComponentID propertyAnimationID);
		void addRigidBody(ComponentID rigidBodyID);
		void addSprite(ComponentID spriteID);
		void addScript(String scriptName);
		Map<ComponentType, ComponentID> getComponents();
	};
}