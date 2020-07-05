#pragma once

#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Queue.hpp"
#include "Subscene2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		uint64_t entityIDCount;
		Vector<Stack<Subscene2D>> subscene2Ds;
	public:
		Scene();
		void addNewSubscene2D();
		Entity2D newEntity2D();
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}