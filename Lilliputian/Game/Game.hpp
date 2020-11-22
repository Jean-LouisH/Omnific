#pragma once

#include <stdint.h>
#include "Scene/Scene.hpp"
#include "../OS.hpp"
#include "../FileAccess.hpp"
#include "../Profiler.hpp"
#include "../Editor/Editor.hpp"
#include "ScriptRegistry.hpp"
#include "../ScriptingAPIs/ScriptingAPIs.hpp"
#include "SceneLoader.hpp"
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
		void initialize();
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
		SceneLoader* sceneLoader;
		Map<String, Script> scripts;
		Vector<Scene> preloadedScenes;
		Stack<Scene> activeSceneStack;

		void initializeEditor();
		void initializeGame(String assetsDirectory);
	};
}