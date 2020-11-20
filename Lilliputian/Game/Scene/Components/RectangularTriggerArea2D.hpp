#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/AABB2D.hpp"
#include "../../../Utilities/Collections/Set.hpp"
#include "../../../Utilities/String.hpp"
#include "../../../Utilities/Constants.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class RectangularTriggerArea2D : public Component
	{
	private:
	public:
		AABB2D aabb;

		Set<EntityID> enteringEntityIDs;
		Set<String> enteringEntityTags;

		bool hasEntityEntered(EntityID entityID);
		bool hasEntityWithTagEntered(String entityTag);
	};
}