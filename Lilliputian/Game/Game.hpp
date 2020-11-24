#pragma once

#include <stdint.h>
#include "Scene/Scene.hpp"
#include "../OS.hpp"
#include "../FileAccess.hpp"
#include "../Profiler.hpp"
#include "../Editor/Editor.hpp"
#include "ScriptRegistry.hpp"
#include "../ScriptingAPIs/ScriptingAPIs.hpp"
#include "SceneSerializer.hpp"
#include "../EngineConfiguration.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Collections/Stack.hpp"
#include "../Utilities/Collections/Map.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class Game
	{
	public:
		Game(
			OS* os,
			Profiler* profiler);
		EngineConfiguration initialize();
		void executeStartLogic();
		void executeInputLogic();
		void executeFrameLogic();
		void executeComputeLogic(unsigned int computeTimeDelta_ms);
		void executeLateLogic();
		void executeFinalLogic();
		void deinitialize();
		ScriptRegistry& getScriptRegistry();
		Scene& getActiveScene();
	private:
		OS* os;
		Profiler* profiler;
		Editor* editor;
		ScriptingAPIs* scriptingAPIs;
		ScriptRegistry* scriptRegistry;
		SceneSerializer* sceneSerializer;
		Map<String, Script> scripts;
		Vector<Scene> preloadedScenes;
		Stack<Scene> activeSceneStack;

		EngineConfiguration initializeEditor();
		EngineConfiguration initializeGame(String assetsDirectory);
	};
}