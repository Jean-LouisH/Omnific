#pragma once

#include <vector>
#include <map>
#include <string>
#include "Scene2D.hpp"
#include "ActionInputs.hpp"
#include "../Utilities/Mathematics/Rectangle.hpp"

namespace Lilliputian
{
	class Game
	{
	private:
		std::vector<Scene2D> activeSubScenes2D;
		std::multimap<std::string, ActionInputs> inputActionMap;
		Rectangle windowDimensions;
	public:
		void initialize();
		void interpretStartLogic();
		void interpretInputLogic();
		void interpretFrameLogic();
		void interpretComputeLogic(unsigned int computeTimeDelta_ms);
		void interpretLateLogic();
		void interpretFinalLogic();
		void buildRenderables();
		void deinitialize();
	};
}