#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Rectangle.hpp"
#include "../../../Utilities/Vector2.hpp"
#include "../../../Utilities/Constants.hpp"

namespace Lilliputian
{
	class Camera2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		Rectangle viewport_px;

		void setViewportWidth(uint32_t width_px);
		void setViewportHeight(uint32_t height_px);
	};
}