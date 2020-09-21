#pragma once

#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/AABB2D.hpp"
#include "../../Utilities/Collections/Set.hpp"
#include "../../Utilities/String.hpp"
#include "../../Utilities/Constants.hpp"

namespace Lilliputian
{
	class BoxTriggerArea2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		AABB2D aabb;

		Set<EntityID> enteringEntityIDs;
		Set<String> enteringEntityTags;

		bool hasEntityEntered(EntityID entityID);
		bool hasEntityWithTagEntered(String entityTag);
	};
}