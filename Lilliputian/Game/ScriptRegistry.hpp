#pragma once

#include "../Utilities/Collections/Map.hpp"
#include "../Game/Scene/Script.hpp"
#include "../UserAPIs/ScriptingAPI.hpp"
#include "../Utilities/String.hpp"

namespace Lilliputian
{
	class ScriptRegistry
	{
	private:
		ScriptingAPI* scriptingAPI;
		Map<String, Script>* scripts;
	public:
		ScriptRegistry(ScriptingAPI* scriptingAPI, Map<String, Script>* scripts);
		Script createNewScript(const char* scriptName);
		void linkScript(Script script);
	};
}

