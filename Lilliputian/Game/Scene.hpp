#pragma once

#include "../Utilities/GenericCollections/Vector.hpp"
#include "../Utilities/GenericCollections/Stack.hpp"
#include "Subscene2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		Vector<Stack<Subscene2D>> subscene2Ds;
	public:
		Scene();
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}