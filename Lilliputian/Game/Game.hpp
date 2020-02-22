#pragma once

#include <vector>
#include <map>
#include <stack>
#include <string>
#include "Scene.hpp"
#include "ActionInputs.hpp"
#include "../UserAPIs/EditorAPI.hpp"

namespace Lilliputian
{
	class Game
	{
	private:
		EditorAPI* editor;
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
		EditorAPI* getEditor();
	};
}