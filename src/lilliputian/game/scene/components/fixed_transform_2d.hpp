#pragma once

#include "../../../utilities/aliases.hpp"
#include "../../../utilities/constants.hpp"
#include "component.hpp"
#include "../../../utilities/transform_2d.hpp"

namespace Lilliputian
{
	class FixedTransform2D : public Component
	{
	public:
		Transform2D transform;
	private:
	};
}