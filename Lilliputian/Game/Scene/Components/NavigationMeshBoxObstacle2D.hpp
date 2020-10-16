#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/AABB2D.hpp"

namespace Lilliputian
{
	class NavigationMeshBoxObstacle2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		AABB2D aabb;
	};
}