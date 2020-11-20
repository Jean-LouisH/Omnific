#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/AABB2D.hpp"
#include "../../../Utilities/Constants.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class RectangularCollider2D : public Component
	{
	private:
	public:
		enum PlatformSide
		{
			PLATFORM_SIDE_NONE,
			PLATFORM_SIDE_LEFT,
			PLATFORM_SIDE_RIGHT,
			PLATFORM_SIDE_TOP,
			PLATFORM_SIDE_BOTTOM
		};

		AABB2D aabb;
		PlatformSide platformSide = PLATFORM_SIDE_NONE;
	};
}