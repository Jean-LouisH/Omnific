#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Transform2D.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class FixedTransform2D : public Component
	{
	public:
		Transform2D transform;
	private:
	};
}