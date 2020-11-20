#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Constants.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class CircleCollider2D : public Component
	{
	private:
	public:
		float radius = 0.0;
	};
}