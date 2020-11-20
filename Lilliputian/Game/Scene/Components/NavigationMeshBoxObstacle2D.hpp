#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/AABB2D.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class NavigationMeshBoxObstacle2D : public Component
	{
	private:
	public:
		AABB2D aabb;
	};
}