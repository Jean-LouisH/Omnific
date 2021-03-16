#pragma once

#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "utilities/vector2.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class ConstantPointForce2D : public Component
	{
	public:
		float force_N;
	private:
	};
}