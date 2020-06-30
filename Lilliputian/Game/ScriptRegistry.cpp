#include "ScriptRegistry.hpp"

Lilliputian::ScriptRegistry::ScriptRegistry(ScriptingAPI* scriptingAPI, Map<String, Script>* scripts)
{
	this->scriptingAPI = scriptingAPI;
	this->scripts = scripts;
}

Lilliputian::Script Lilliputian::ScriptRegistry::createNewScript(const char* scriptName)
{
	return Script(this->scriptingAPI, scriptName);
}

void Lilliputian::ScriptRegistry::linkScript(Script script)
{
	this->scripts->emplace(script.name, script);
}