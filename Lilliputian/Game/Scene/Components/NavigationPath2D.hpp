#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Collections/Vector.hpp"
#include "../../../Utilities/Vector2.hpp"

namespace Lilliputian
{
	class NavigationPath2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		Vector<Vector2> waypoints;
	};
}