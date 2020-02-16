#pragma once

#include <vector>
#include <stack>
#include "Subscene2D.hpp"

namespace Lilliputian
{
	class Scene
	{
	private:
		std::vector<std::stack<Subscene2D>> subscene2Ds;
	public:
		Scene();
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}