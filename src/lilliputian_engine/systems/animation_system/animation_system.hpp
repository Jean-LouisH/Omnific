#pragma once

#include "game/scene/scene_forest.hpp"

namespace Lilliputian
{
	class AnimationSystem
	{
	public:
		AnimationSystem();
		~AnimationSystem();
		void process(SceneForest& scene);
	private:
	};
}

