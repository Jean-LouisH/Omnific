#pragma once

#include <stdint.h>
#include "scene/scene_forest.hpp"
#include "os.hpp"
#include "file_access.hpp"
#include "profiler.hpp"
#include "script_registry.hpp"
#include "scripting_apis/scripting_apis.hpp"
#include "scene_serializer.hpp"
#include "boot_configuration.hpp"
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
		void initialize();
		void executeOnStartMethods();
		void executeOnInputMethods();
		void executeOnFrameMethods();
		void executeOnComputeMethods();
		void executeOnLateMethods();
		void executeOnFinalMethods();
		void deinitialize();
		ScriptRegistry& getScriptRegistry();
		SceneForest& getActiveScene();
		BootConfiguration& configuration();
	private:
		OS* os = nullptr;
		Profiler* profiler = nullptr;
		BootConfiguration* _configuration = nullptr;
		ScriptingAPIs* scriptingAPIs = nullptr;
		ScriptRegistry* scriptRegistry = nullptr;
		SceneSerializer* sceneSerializer = nullptr;
		Map<String, Script> scripts;
		Vector<SceneForest> preloadedScenes;
		Stack<SceneForest> activeSceneStack;
	};
}