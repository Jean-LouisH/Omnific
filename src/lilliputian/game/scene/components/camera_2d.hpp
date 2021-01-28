#pragma once

#include "../../../utilities/aliases.hpp"
#include "../../../utilities/rectangle.hpp"
#include "../../../utilities/vector2.hpp"
#include "../../../utilities/constants.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class Camera2D : public Component
	{
	public:
		Rectangle viewport_px;

		void setViewportWidth(uint32_t width_px);
		void setViewportHeight(uint32_t height_px);
	private:
	};
}