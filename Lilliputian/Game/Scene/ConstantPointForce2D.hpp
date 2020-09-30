#pragma once

#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/Constants.hpp"
#include "../../Utilities/Vector2.hpp"

namespace Lilliputian
{
	class ConstantPointForce2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		float force_N;
	};
}