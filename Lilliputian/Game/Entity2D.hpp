#pragma once

#include "Script.hpp"
#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Transform2D.hpp"
#include "BoxCollider2D.hpp"
#include "Camera2D.hpp"
#include "RigidBody2D.hpp"
#include "Sprite2D.hpp"

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
		};

		EntityID id;
		Vector<String> scriptNames;
		Entity2D* parentEntity;
		Vector<Entity2D> childrenEntities;
		Map<ComponentType, ComponentID> components;
	public:
		Transform2D transform2D;

		Entity2D(EntityID id);
		void addParentEntity(Entity2D parentEntity);
		void addChildEntity(Entity2D childEntity);
		void addBoxCollider(ComponentID boxCollider);
		void addCamera(ComponentID camera);
		void addRigidBody(ComponentID rigidBody);
		void addSprite(ComponentID sprite);
		void addScript(String scriptName);
		Map<ComponentType, ComponentID> getComponents();
	};
}