#pragma once

#include "Scene.hpp"
#include "ActionInputs.hpp"
#include "../UserAPIs/EditorAPI.hpp"
#include "../Utilities/GenericCollections/Vector.hpp"
#include "../Utilities/GenericCollections/Stack.hpp"
#include "../Utilities/GenericCollections/Map.hpp"
#include "../Utilities/GenericCollections/String.hpp"

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
	};
}