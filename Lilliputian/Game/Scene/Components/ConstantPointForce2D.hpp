#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Vector2.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class ConstantPointForce2D : public Component
	{
	public:
		float force_N;
	private:
	};
}