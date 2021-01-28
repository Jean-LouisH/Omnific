#pragma once

#include "../../../utilities/aliases.hpp"
#include "../../../utilities/aabb_2d.hpp"
#include "../../../utilities/collections/set.hpp"
#include "../../../utilities/string.hpp"
#include "../../../utilities/constants.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class RectangularTriggerArea2D : public Component
	{
	public:
		AABB2D aabb;

		Set<EntityID> enteringEntityIDs;
		Set<String> enteringEntityTags;

		bool hasEntityEntered(EntityID entityID);
		bool hasEntityWithTagEntered(String entityTag);
	private:
	};
}