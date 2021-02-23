#pragma once

#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "utilities/aabb_2d.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class NavigationMeshBoxObstacle2D : public Component
	{
	public:
		AABB2D aabb;
	private:
	};
}