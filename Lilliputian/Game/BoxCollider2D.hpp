#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/AABB2D.hpp"

namespace Lilliputian
{
	class BoxCollider2D
	{
	private:
		EntityID entityID;
	public:
		AABB2D aabb;

		BoxCollider2D(EntityID entityID);
		EntityID getEntityID();
	};
}