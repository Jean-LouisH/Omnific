#pragma once

#include "../utilities/collections/map.hpp"
#include "../game/scene/script.hpp"
#include "../scripting_apis/scripting_apis.hpp"
#include "../utilities/string.hpp"

namespace Lilliputian
{
	class ScriptRegistry
	{
	public:
		ScriptRegistry(ScriptingAPIs* scriptingAPIs, Map<String, Script>* scripts);
		Script createNewScript(const char* scriptName);
		void linkScript(Script script);
	private:
		ScriptingAPIs* scriptingAPIs;
		Map<String, Script>* scripts;
	};
}

