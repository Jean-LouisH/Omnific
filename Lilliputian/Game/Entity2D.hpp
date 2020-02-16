#pragma once

#include "Script.hpp"
#include <vector>
#include "../Utilities/Definitions/Aliases.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	private:
		std::vector<Script> scripts;
		Entity2D* parentEntity;
		std::vector<Entity2D> childrenEntities;
	public:
		Entity2D();
		void addParentEntity(Entity2D parentEntity);
		void addChildEntity(Entity2D childEntity);
		void addScript(Script script);
		void executeFrameLogic();
	};
}