#pragma once

#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
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