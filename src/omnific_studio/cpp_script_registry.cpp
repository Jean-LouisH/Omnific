// // MIT License
// // 
// // Copyright (c) 2020 Jean-Louis Haywood
// // 
// // Permission is hereby granted, free of charge, to any person obtaining a copy
// // of this software and associated documentation files (the "Software"), to deal
// // in the Software without restriction, including without limitation the rights
// // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// // copies of the Software, and to permit persons to whom the Software is
// // furnished to do so, subject to the following conditions:
// // 
// // The above copyright notice and this permission notice shall be included in all
// // copies or substantial portions of the Software.
// // 
// // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// // SOFTWARE.

// #include "cpp_script_registry.hpp"
// #include "systems/cpp_scripting_system/cpp_entity_context.hpp"
// #include <scene/components/script_collection.hpp>

// Omnific::CPPScriptRegistry* Omnific::CPPScriptRegistry::instance = nullptr;

// void Omnific::CPPScriptRegistry::loadScriptInstances()
// {
// 	CPPScriptRegistry* registry = getInstance();

// 	registry->initialize();
// 	registry->cppScriptInstances.clear();

// 	for (auto it : CPPEntityContext::getScene()->getSceneLayers())
// 		for (std::shared_ptr<ScriptCollection> scriptCollection : it.second->getComponentsByType<ScriptCollection>())
// 			for (std::shared_ptr<Script> script : scriptCollection->scripts)
// 				if (script->getType() == Omnific::CPPScriptInstance::TYPE_STRING)
// 					if (registry->cppScriptDefinitions.count(script->getName()))
// 						registry->cppScriptInstances.emplace(
// 							script->getName() + std::to_string(scriptCollection->getEntityID()),
// 							std::shared_ptr<CPPScriptInstance>(registry->cppScriptDefinitions.at(script->getName())->instance())
// 						);
// }

// std::unordered_map<std::string, std::shared_ptr<Omnific::CPPScriptInstance>> Omnific::CPPScriptRegistry::getScriptInstances()
// {
// 	return getInstance()->cppScriptInstances;
// }

// void Omnific::CPPScriptRegistry::finalize()
// {

// }

// Omnific::CPPScriptRegistry* Omnific::CPPScriptRegistry::getInstance()
// {
// 	if (instance == nullptr)
// 		instance = new CPPScriptRegistry();
// 	return instance;
// }