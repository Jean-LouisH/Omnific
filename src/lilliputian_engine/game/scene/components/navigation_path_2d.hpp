#pragma once

#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/vector2.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class NavigationPath2D : public Component
	{
	public:
		Vector<Vector2> waypoints;
	private:
	};
}