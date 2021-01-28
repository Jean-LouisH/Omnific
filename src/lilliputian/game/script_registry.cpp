#include "script_registry.hpp"

Lilliputian::ScriptRegistry::ScriptRegistry(ScriptingAPIs* scriptingAPIs, Map<String, Script>* scripts)
{
	this->scriptingAPIs = scriptingAPIs;
	this->scripts = scripts;
}

Lilliputian::Script Lilliputian::ScriptRegistry::createNewScript(const char* scriptName)
{
	return Script(this->scriptingAPIs, scriptName);
}

void Lilliputian::ScriptRegistry::linkScript(Script script)
{
	this->scripts->emplace(script.name, script);
}