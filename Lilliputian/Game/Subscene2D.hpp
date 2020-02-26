#pragma once

#include "Entity2D.hpp"
#include "../Utilities/Mathematics/Rectangle.hpp"
#include "../Utilities/GenericCollections/Vector.hpp"

namespace Lilliputian
{
	class Subscene2D
	{
	private:
		Vector<Entity2D> entities2D;
		Rectangle windowDimensions;
	public:
		Subscene2D();
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}