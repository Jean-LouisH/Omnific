#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/Vector2.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class ConstantDirectionalForce2D : public Component
	{
	private:
	public:
		Vector2 force_N;
	};
}