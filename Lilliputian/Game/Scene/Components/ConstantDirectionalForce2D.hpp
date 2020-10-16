#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Vector2.hpp"

namespace Lilliputian
{
	class ConstantDirectionalForce2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		Vector2 force_N;
	};
}