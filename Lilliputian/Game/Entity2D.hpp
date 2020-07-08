#pragma once

#include "Script.hpp"
#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Transform2D.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	private:
		enum Components
		{
			COMPONENT_NONE,
			COMPONENT_CAMERA_2D,
			COMPONENT_RIGID_BODY_2D,
			COMPONENT_SPRITE_2D,
			COMPONENT_BOX_COLLIDER_2D,
			COMPONENT_PROPERTY_ANIMATION
		};

		EntityID id;
		Vector<String> scriptNames;
		EntityID parentEntity;
		Vector<EntityID> childrenEntities;
		Map<ComponentType, ComponentID> components;
	public:
		Transform2D transform2D;

		Entity2D(EntityID id);
		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addBoxCollider(ComponentID boxColliderID);
		void addCamera(ComponentID cameraID);
		void addRigidBody(ComponentID rigidBodyID);
		void addSprite(ComponentID spriteID);
		void addPropertyAnimation(ComponentID propertyAnimationID);
		void addScript(String scriptName);
		Map<ComponentType, ComponentID> getComponents();
	};
}