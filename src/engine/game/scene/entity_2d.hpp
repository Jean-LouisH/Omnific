#pragma once

#include "script.hpp"
#include "components/component_variant.hpp"
#include "utilities/aliases.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	public:
		EntityID ID = NO_ENTITY;
		EntityID parentID = NO_ENTITY;
		Vector<EntityID> childIDs;
		Vector<String> tags;
		Vector<String> scriptNames;
		Map<ComponentVariant::Type, ComponentID> components;

		void addParentEntity(EntityID parentEntityID);
		void addChildEntity(EntityID childEntityID);
		void addComponent(ComponentVariant::Type type, ComponentID ID);
		void addScript(String scriptName);
	private:
	};
}