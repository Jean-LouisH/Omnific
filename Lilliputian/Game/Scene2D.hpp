#pragma once

#include <vector>
#include "Entity2D.hpp"
#include "../Utilities/Mathematics/Rectangle.hpp"

namespace Lilliputian
{
	class Scene2D
	{
	private:
		std::vector<Entity2D> entities2D;
		Rectangle windowDimensions;
	public:
		Scene2D();
		void addEntity2D(Entity2D entity2D);
	};
}