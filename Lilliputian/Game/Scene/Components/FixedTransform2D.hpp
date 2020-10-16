#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Transform2D.hpp"

namespace Lilliputian
{
	class FixedTransform2D
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;
		Transform2D transform;
	};
}