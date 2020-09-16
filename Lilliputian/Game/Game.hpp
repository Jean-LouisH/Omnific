#pragma once

#include <stdint.h>
#include "Scene/Scene.hpp"
#include "../OSWindow.hpp"
#include "../FileSystem.hpp"
#include "../Profiler.hpp"
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
	private:
		OSWindow* osWindow;
		FileSystem* fileSystem;
		Profiler* profiler;
		ScriptingAPIs* scriptingAPIs;
		ScriptRegistry* scriptRegistry;
		SceneLoader* sceneLoader;
		Map<String, Script> scripts;
		Vector<Scene> preloadedScenes;
		Stack<Scene> activeSceneStack;
	public:
		Game(
			OSWindow* osWindow,
			FileSystem* fileSystem,
			Profiler* profiler);
		void initialize(String assetsDirectory, String entryScenePath);
		void executeStartLogic();
		void executeInputLogic();
		void executeFrameLogic();
		void executeComputeLogic(unsigned int computeTimeDelta_ms);
		void executeLateLogic();
		void executeFinalLogic();
		void deinitialize();
		ScriptRegistry& getScriptRegistry();
		Scene* getActiveScene();
	};
}