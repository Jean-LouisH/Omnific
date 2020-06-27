#pragma once

#include <stdint.h>
#include "Scene.hpp"
#include "../UserAPIs/EditorAPI.hpp"
#include "../UserAPIs/InputAPI.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Map.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class Game
	{
	private:
		EditorAPI* editorAPI;
		InputAPI* inputAPI;
		uint8_t entrySceneIndex;
		Vector<Scene> scenes;
		Stack<Scene> activeScenes;
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
		EditorAPI* getEditorAPI();
		InputAPI* getInputAPI();
		Scene* getActiveScene();
	};
}