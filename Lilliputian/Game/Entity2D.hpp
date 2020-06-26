#pragma once

#include "Script.hpp"
#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"

namespace Lilliputian
{
	class Entity2D
	{
	private:
		EntityID id;
		Vector<Script> scripts;
		Entity2D* parentEntity;
		Vector<Entity2D> childrenEntities;
	public:
		Entity2D(EntityID id);
		void addParentEntity(Entity2D parentEntity);
		void addChildEntity(Entity2D childEntity);
		void addScript(Script script);
		void executeFrameLogic();
	};
}