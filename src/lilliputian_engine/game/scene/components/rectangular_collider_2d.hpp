#pragma once

#include "utilities/aliases.hpp"
#include "utilities/aabb_2d.hpp"
#include "utilities/constants.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class RectangularCollider2D : public Component
	{
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
	private:
	};
}