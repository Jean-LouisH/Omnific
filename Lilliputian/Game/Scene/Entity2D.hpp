#pragma once

#include "Script.hpp"
#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "../../Utilities/Transform2D.hpp"
#include "../../Utilities/Constants.hpp"
#include "../../Utilities/String.hpp"

namespace Lilliputian
{
	enum ComponentType
	{
		COMPONENT_TYPE_NONE,
		COMPONENT_TYPE_ANIMATED_SPRITE_2D,
		COMPONENT_TYPE_AUDIO_LISTENER_2D,
		COMPONENT_TYPE_BOX_COLLIDER_2D,
		COMPONENT_TYPE_BOX_TRIGGER_AREA_2D,
		COMPONENT_TYPE_CAMERA_2D,
		COMPONENT_TYPE_CIRCLE_COLLIDER_2D,
		COMPONENT_TYPE_CIRCULAR_ITEM_LIST_2D,
		COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D,
		COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D,
		COMPONENT_TYPE_COUNTDOWN_TIMER,
		COMPONENT_TYPE_FIXED_TRANSFORM_2D,
		COMPONENT_TYPE_ITEM_LIST,
		COMPONENT_TYPE_NAVIGATION_MESH_2D,
		COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D,
		COMPONENT_TYPE_NAVIGATION_MESH_OBSTACLE_2D,
		COMPONENT_TYPE_NAVIGATION_PATH_2D,
		COMPONENT_TYPE_NEURAL_NETWORK,
		COMPONENT_TYPE_PROPERTY_ANIMATION,
		COMPONENT_TYPE_RECTANGULAR_MESH_2D,
		COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D,
		COMPONENT_TYPE_RIGID_BODY_2D,
		COMPONENT_TYPE_SPRITE_2D,
		COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D,
		COMPONENT_TYPE_SYNTHESIZED_AUDIO_SOURCE_2D,
		COMPONENT_TYPE_TEXT_LABEL_2D,
		COMPONENT_TYPE_TEXTURED_PROGRESS_BAR_2D
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
		Vector<String> tags;

		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addComponent(ComponentType type, ComponentID ID);
		void addScript(String scriptName);
		Map<ComponentType, ComponentID> getComponents();
	};
}