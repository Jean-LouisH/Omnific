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

#include "cpp_script_assembly.hpp"
#include "cpp_script_registry.hpp"
#include "systems/cpp_scripting_system/cpp_entity_context.hpp"
#include "foundations/singletons/platform/platform.hpp"

#include <scene/components/script_collection.hpp>

void loadScriptInstances()
{
	Omnific::CPPScriptRegistry::loadScriptInstances();
}

void onStart()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeQueuedMethods(it.second->getStartEntityQueue(), it.second, "onStart");
}

void onInput()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeUpdateMethods(it.second, "onInput");
}

void onEarly()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeUpdateMethods(it.second, "onEarly");
}

void onLogic()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeUpdateMethods(it.second, "onLogic");
}

void onCompute()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeUpdateMethods(it.second, "onCompute");
}

void onLate()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeUpdateMethods(it.second, "onLate");
}

void onFinish()
{
	for (auto it : Omnific::CPPEntityContext::getScene()->getSceneLayers())
		executeQueuedMethods(it.second->getFinishEntityQueue(), it.second, "onFinish");
}

void bindAndCall(std::shared_ptr<Omnific::ScriptCollection> scriptCollection,
	Omnific::SceneLayerID sceneLayerID,
	Omnific::EntityID entityID,
	std::string methodName)
{
	for (auto script : scriptCollection->scripts)
	{
		std::string scriptInstanceName = script->getName() + std::to_string(entityID);
		std::unordered_map<std::string, std::shared_ptr<Omnific::CPPScriptInstance>> scriptInstances = Omnific::CPPScriptRegistry::getScriptInstances();

		if (scriptInstances.count(scriptInstanceName))
		{
			std::shared_ptr<Omnific::CPPScriptInstance> scriptInstance = scriptInstances.at(scriptInstanceName);

			Omnific::CPPEntityContext::bindEntity(
				sceneLayerID,
				entityID);

			if (methodName == "onStart")
				scriptInstance->onStart();
			else if (methodName == "onInput")
				scriptInstance->onInput();
			else if (methodName == "onEarly")
				scriptInstance->onEarly();
			else if (methodName == "onLogic")
				scriptInstance->onLogic();
			else if (methodName == "onCompute")
				scriptInstance->onCompute();
			else if (methodName == "onLate")
				scriptInstance->onLate();
			else if (methodName == "onFinish")
				scriptInstance->onFinish();
		}
	}
}

void executeQueuedMethods(
	std::queue<Omnific::EntityID> entityQueue,
	std::shared_ptr<Omnific::SceneLayer> sceneLayer,
	std::string methodName)
{
	while (!entityQueue.empty())
	{
		std::shared_ptr<Omnific::Entity> entity = sceneLayer->getEntity(entityQueue.front());
		std::shared_ptr<Omnific::ScriptCollection> scriptCollection = sceneLayer->getComponentByType<Omnific::ScriptCollection>(entity->getID());
		if (scriptCollection != nullptr)
		{
			bindAndCall(scriptCollection, sceneLayer->getID(), scriptCollection->getEntityID(), methodName);
		}
		entityQueue.pop();
	}
}

void executeUpdateMethods(
	std::shared_ptr<Omnific::SceneLayer> sceneLayer,
	std::string methodName)
{
	for (auto scriptCollection : sceneLayer->getComponentsByType<Omnific::ScriptCollection>())
		bindAndCall(scriptCollection, sceneLayer->getID(), scriptCollection->getEntityID(), methodName);
}