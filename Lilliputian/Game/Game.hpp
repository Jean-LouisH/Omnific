#pragma once

#include "Scene.hpp"
#include "ActionInputs.hpp"
#include "../UserAPIs/EditorAPI.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Map.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class Game
	{
	private:
		EditorAPI* editor;
		Scene entryScene;
		Vector<Scene> scenes;
		Stack<Scene> activeScenes;
		Multimap<String, ActionInputs> inputActionMap;
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
		Scene* getActiveScene();
	};
}