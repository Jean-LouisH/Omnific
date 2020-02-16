#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include "Scene.hpp"
#include "ActionInputs.hpp"
#include "../Editor/Editor.hpp"

namespace Lilliputian
{
	class Game
	{
	private:
		Editor* editor;
		Scene entryScene;
		std::vector<Scene> scenes;
		std::stack<Scene> activeScenes;
		std::multimap<std::string, ActionInputs> inputActionMap;
	public:
		Game();
		void initialize();
		void executeStartLogic();
		void executeInputLogic();
		void executeFrameLogic();
		void executeComputeLogic(unsigned int computeTimeDelta_ms);
		void executeLateLogic();
		void executeFinalLogic();
		void deinitialize();
		Editor* getEditor();
	};
}