// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "cpp_script_registry.hpp"
#include "systems/scripting_system/script_context.hpp"

Omnia::CPPScriptRegistry* Omnia::CPPScriptRegistry::instance = nullptr;

void Omnia::CPPScriptRegistry::loadScriptInstances()
{
	CPPScriptRegistry* registry = getInstance();

	registry->initialize();
	registry->cppScriptInstances.clear();

	for (auto it : ScriptContext::getScene().getSceneTrees())
		for (std::shared_ptr<ScriptCollection> scriptCollection : it.second->getComponentsByType<ScriptCollection>())
			for (std::shared_ptr<Script> script : scriptCollection->scripts)
				if (script->getType() == Omnia::CPPScript::TYPE_STRING)
					if (registry->cppScriptDefinitions.count(script->getName()))
						registry->cppScriptInstances.emplace(
							script->getName() + std::to_string(scriptCollection->getEntityID()),
							std::shared_ptr<CPPScript>(registry->cppScriptDefinitions.at(script->getName())->copy())
						);
}

std::unordered_map<std::string, std::shared_ptr<Omnia::CPPScript>> Omnia::CPPScriptRegistry::getScriptInstances()
{
	return getInstance()->cppScriptInstances;
}

void Omnia::CPPScriptRegistry::finalize()
{

}

Omnia::CPPScriptRegistry* Omnia::CPPScriptRegistry::getInstance()
{
	if (instance == nullptr)
		instance = new CPPScriptRegistry();
	return instance;
}