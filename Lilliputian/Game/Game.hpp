#pragma once

#include <stdint.h>
#include "Scene.hpp"
#include "ScriptRegistry.hpp"
#include "../UserAPIs/GameAPI.hpp"
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
		GameAPI* gameAPI;
		InputAPI* inputAPI;
		ScriptRegistry* scriptRegistry;
		Map<String, Script> scripts;
		Vector<Scene> preloadedScenes;
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
		InputAPI* getInputAPI();
		ScriptRegistry* getScriptRegistry();
		Scene* getActiveScene();
	};
}