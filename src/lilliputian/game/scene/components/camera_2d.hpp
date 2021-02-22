#pragma once

#include "utilities/aliases.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/aabb_2d.hpp"
#include "utilities/vector2.hpp"
#include "utilities/constants.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class Camera2D : public Component
	{
	public:
		Camera2D();
		void resetAspect();
		Rectangle getViewportDimensions();
		void setViewportWidth(uint32_t width_px);
		void setViewportHeight(uint32_t height_px);
		void setLimits(float left, float bottom, float top, float right);
		void setKeepAspect(bool value);
		void setIsStreaming(bool value);
	private:
		const float defaultAspect = 1920.0 / 1080.0;
		float aspect = defaultAspect;
		AABB2D limits_px;
		Rectangle viewport_px;
		bool keepAspect = true;
		bool isStreaming = true;
	};
}