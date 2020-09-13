#pragma once

#include "Script.hpp"
#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "../../Utilities/Transform2D.hpp"
#include "../../Utilities/Constants.hpp"

namespace Lilliputian
{
	enum ComponentType
	{
		COMPONENT_TYPE_NONE,
		COMPONENT_TYPE_ANIMATED_SPRITE_2D,
		COMPONENT_TYPE_AUDIO_LISTENER_2D,
		COMPONENT_TYPE_BOX_COLLIDER_2D,
		COMPONENT_TYPE_CAMERA_2D,
		COMPONENT_TYPE_PROPERTY_ANIMATION,
		COMPONENT_TYPE_RIGID_BODY_2D,
		COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D
	};

	class Entity2D
	{
	private:
		Vector<String> scriptNames;
		Map<ComponentType, ComponentID> components;
	public:
		Transform2D transform2D;
		EntityID id = NO_ENTITY;
		EntityID parentEntity = NO_ENTITY;
		Vector<EntityID> childrenEntities;

		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addComponent(ComponentType type, ComponentID ID);
		void addScript(String scriptName);
		Map<ComponentType, ComponentID> getComponents();
	};
}