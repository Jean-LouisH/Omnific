#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Collections/Vector.hpp"
#include "../../../Utilities/Vector2.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class NavigationPath2D : public Component
	{
	public:
		Vector<Vector2> waypoints;
	private:
	};
}