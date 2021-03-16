#pragma once

#include "game/scene/scene_forest.hpp"

namespace Lilliputian
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		void process(SceneForest& scene);
	private:
	};
}

