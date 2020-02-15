#pragma once

#include "Script.hpp"
#include <vector>
#include "../Utilities/Definitions/Aliases.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	private:
		EntityID entityID;
		std::vector<Script> scripts;
		EntityID parentEntity;
		std::vector<EntityID> childrenEntity;
	public:
		Entity2D();
		void addParentEntity(Entity2D parentEntity);
		void addChildEntity(Entity2D childEntity);
		void addScript(Script script);
	};
}