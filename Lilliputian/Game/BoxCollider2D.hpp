#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/AABB2D.hpp"
#include "../Utilities/Constants.hpp"

namespace Lilliputian
{
	class BoxCollider2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		AABB2D aabb;
	};
}