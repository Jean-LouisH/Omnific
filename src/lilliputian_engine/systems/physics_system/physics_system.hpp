#pragma once

#include "game/scene/scene.hpp"

namespace Lilliputian
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();
		void process(Scene& scene);
	private:
	};
}

