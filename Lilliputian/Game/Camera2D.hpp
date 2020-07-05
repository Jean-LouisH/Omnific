#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Rectangle.hpp"
#include "../Utilities/Vector2.hpp"

namespace Lilliputian
{
	class Camera2D
	{
	private:
		EntityID entityID;
	public:
		Rectangle viewport_px;
		Vector2 offset_px;

		Camera2D(EntityID entityID);
		EntityID getEntityID();
	};
}