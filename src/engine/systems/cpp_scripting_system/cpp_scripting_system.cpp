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

#include "cpp_scripting_system.hpp"
#include "cpp_entity_context.hpp"
#include "scene/components/script_collection.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <customization/class_registry.hpp>
#include <iostream>

typedef void(ScriptProcedure)(void);

void Omnific::CPPScriptingSystem::initialize()
{
	Logger& logger = Platform::get_logger();
	logger.write("Initializing C++ Scripting System...");
}

void Omnific::CPPScriptingSystem::load_script_modules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<std::string, std::shared_ptr<CPPScriptInstance>> cpp_script_definitions;
		for (auto it : ClassRegistry::query_all<CPPScriptInstance>())
		{
			std::shared_ptr<CPPScriptInstance> cpp_script_definition = std::dynamic_pointer_cast<CPPScriptInstance>(it.second);

			cpp_script_definitions.emplace(
				it.first,
				cpp_script_definition
			);
		}

		this->cpp_script_instances.clear();

		for (auto it : scene->get_scene_layers())
		{
			for (std::shared_ptr<ScriptCollection> script_collection : it.second->get_components_by_type<ScriptCollection>())
			{
				for (std::shared_ptr<Script> script : script_collection->scripts)
				{
					if (script->get_language_name() == "CPP")
					{
						if (cpp_script_definitions.count(script->get_name()))
						{
							this->cpp_script_instances.emplace(
								script->get_name() + std::to_string(script_collection->get_entity_id()),
								std::dynamic_pointer_cast<CPPScriptInstance>(std::shared_ptr<Registerable>(cpp_script_definitions.at(script->get_name())->instance()))
							);
						}
					}
				}
			}
		}
	}
}

void Omnific::CPPScriptingSystem::on_input(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
	for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
		execute_update_methods(it.second, "on_input");
	}
}

void Omnific::CPPScriptingSystem::on_entity_start(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
	for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
		execute_queued_methods(it.second->get_start_entity_queue(), it.second, "on_entity_start");
	}
}

void Omnific::CPPScriptingSystem::on_early_update(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_update_methods(it.second, "on_early_update");
	}
}

void Omnific::CPPScriptingSystem::on_update(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_update_methods(it.second, "on_update");
	}
}

void Omnific::CPPScriptingSystem::on_fixed_update(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_update_methods(it.second, "on_fixed_update");
	}
}

void Omnific::CPPScriptingSystem::on_late_update(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_update_methods(it.second, "on_late_update");
	}
}

void Omnific::CPPScriptingSystem::on_entity_finish(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_queued_methods(it.second->get_finish_entity_queue(), it.second, "on_entity_finish");
	}
}

void Omnific::CPPScriptingSystem::on_output(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto it : CPPEntityContext::get_scene()->get_scene_layers())
			execute_update_methods(it.second, "on_output");
	}
}

void Omnific::CPPScriptingSystem::bind_and_call(
	std::shared_ptr<ScriptCollection> scriptCollection,
	SceneLayerID sceneLayerID,
	EntityID entityID,
	std::string methodName)
{
	for (auto script : scriptCollection->scripts)
	{
		std::string script_instance_name = script->get_name() + std::to_string(entityID);

		if (this->cpp_script_instances.count(script_instance_name))
		{
			std::shared_ptr<CPPScriptInstance> script_instance = this->cpp_script_instances.at(script_instance_name);

			Omnific::CPPEntityContext::bind_entity(
				sceneLayerID,
				entityID);

			if (methodName == "on_entity_start")
				script_instance->on_entity_start();
			else if (methodName == "on_input")
				script_instance->on_input();
			else if (methodName == "on_early_update")
				script_instance->on_early_update();
			else if (methodName == "on_update")
				script_instance->on_update();
			else if (methodName == "on_fixed_update")
				script_instance->on_fixed_update();
			else if (methodName == "on_late_update")
				script_instance->on_late_update();
			else if (methodName == "on_entity_finish")
				script_instance->on_entity_finish();
            else if (methodName == "on_output")
				script_instance->on_output();
		}
	}
}

void Omnific::CPPScriptingSystem::execute_queued_methods(
	std::queue<EntityID> entityQueue,
	std::shared_ptr<SceneLayer> sceneLayer,
	std::string methodName)
{
	while (!entityQueue.empty())
	{
		std::shared_ptr<Entity> entity = sceneLayer->get_entity(entityQueue.front());
		std::shared_ptr<ScriptCollection> scriptCollection = sceneLayer->get_component_by_type<ScriptCollection>(entity->get_id());
		if (scriptCollection != nullptr)
		{
			bind_and_call(scriptCollection, sceneLayer->get_id(), scriptCollection->get_entity_id(), methodName);
		}
		entityQueue.pop();
	}
}

void Omnific::CPPScriptingSystem::execute_update_methods(
	std::shared_ptr<SceneLayer> sceneLayer,
	std::string methodName)
{
	for (auto scriptCollection : sceneLayer->get_components_by_type<ScriptCollection>())
		bind_and_call(scriptCollection, sceneLayer->get_id(), scriptCollection->get_entity_id(), methodName);
}

void Omnific::CPPScriptingSystem::finalize()
{

}