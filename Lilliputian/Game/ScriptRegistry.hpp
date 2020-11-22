#pragma once

#include "../Utilities/Collections/Map.hpp"
#include "../Game/Scene/Script.hpp"
#include "../ScriptingAPIs/ScriptingAPIs.hpp"
#include "../Utilities/String.hpp"

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

