#pragma once

#include "script.hpp"
#include "components/component.hpp"
#include "utilities/aliases.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/transform_2d.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	public:
		Transform2D transform2D;
		EntityID id = NO_ENTITY;
		EntityID parentEntity = NO_ENTITY;
		Vector<EntityID> childrenEntities;
		Vector<String> tags;
		Vector<String> scriptNames;
		Map<Component::ComponentType, ComponentID> components;

		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addComponent(Component::ComponentType type, ComponentID ID);
		void addScript(String scriptName);
	private:
	};
}