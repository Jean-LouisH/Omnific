#pragma once

#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "utilities/vector2.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class ConstantDirectionalForce2D : public Component
	{
	public:
		Vector2 force_N;
	private:
	};
}