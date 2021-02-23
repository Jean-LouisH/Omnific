#pragma once

#include <stdint.h>
#include "scene/scene.hpp"
#include "os.hpp"
#include "file_access.hpp"
#include "profiler.hpp"
#include "script_registry.hpp"
#include "scripting_apis/scripting_apis.hpp"
#include "scene_serializer.hpp"
#include "engine_configuration.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/stack.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/string.hpp"

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