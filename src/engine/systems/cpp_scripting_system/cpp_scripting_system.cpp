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
#include <foundations/singletons/profiler.hpp>
#include <iostream>

#define CPP_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_input_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_entity_start_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_early_update_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_update_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_fixed_update_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_late_update_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_entity_finish_frame_time"
#define CPP_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "cpp_scripting_system_on_output_frame_time"

typedef void(ScriptProcedure)(void);

void Omnific::CPPScriptingSystem::initialize()
{
	this->is_initialized = true;
	Logger& logger = Platform::get_logger();
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_input_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_entity_start_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_early_update_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_update_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_fixed_update_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_late_update_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_entity_finish_frame_time"});
	Profiler::add_clock(CPP_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"cpp_scripting_system", "on_output_frame_time"});
	logger.write("Initializing C++ Scripting System...");
}

void Omnific::CPPScriptingSystem::load_script_modules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<std::string, std::shared_ptr<CPPScriptInstance>> cpp_script_definitions;
		for (auto& [class_name, registerable] : ClassRegistry::query_all<CPPScriptInstance>())
		{
			std::shared_ptr<CPPScriptInstance> cpp_script_definition = std::dynamic_pointer_cast<CPPScriptInstance>(registerable);

			cpp_script_definitions.emplace(
				class_name,
				cpp_script_definition
			);
		}

		this->cpp_script_instances.clear();

		for (auto& [id, scene_layer] : scene->get_scene_layers())
		{
			for (std::shared_ptr<ScriptCollection>& script_collection : scene_layer->get_components_by_type<ScriptCollection>())
			{
				for (std::shared_ptr<Script>& script : script_collection->scripts)
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
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_input");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_entity_start(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_queued_methods(scene_layer->get_start_entity_queue(), scene_layer, "on_entity_start");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_early_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_early_update");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_update");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_fixed_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_fixed_update");
	}
	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_late_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_late_update");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_entity_finish(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_queued_methods(scene_layer->get_finish_entity_queue(), scene_layer, "on_entity_finish");
	}
	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::on_output(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CPP_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
	{
		for (auto& [id, scene_layer] : scene->get_scene_layers())
			execute_update_methods(scene_layer, "on_output");
	}

	frame_time_clock->set_end();
}

void Omnific::CPPScriptingSystem::bind_and_call(
	std::shared_ptr<ScriptCollection> scriptCollection,
	SceneLayerID sceneLayerID,
	EntityID entityID,
	std::string method_name)
{
	std::shared_ptr<Script>* scripts = scriptCollection->scripts.data();
	int scripts_count = scriptCollection->scripts.size();

	for (int i = 0; i < scripts_count; ++i)
	{
		std::shared_ptr<Script> script = scripts[i];
		std::string script_instance_name = script->get_name() + std::to_string(entityID);

		if (this->cpp_script_instances.count(script_instance_name))
		{
			std::shared_ptr<CPPScriptInstance> script_instance = this->cpp_script_instances.at(script_instance_name);

			Omnific::CPPEntityContext::bind_entity(
				sceneLayerID,
				entityID);

			CPPEntityContext::bind_time_delta(Profiler::get_clock(TOTAL_LOOP_FRAME_TIME_CLOCK_NAME)->get_delta_in_seconds());

			switch (this->loop_event_indices.at(method_name))
			{
				case LoopEventIndex::ON_ENTITY_START: script_instance->on_entity_start(); break;
				case LoopEventIndex::ON_INPUT: script_instance->on_input(); break;
				case LoopEventIndex::ON_EARLY_UPDATE: script_instance->on_early_update(); break;
				case LoopEventIndex::ON_UPDATE: script_instance->on_update(); break;
				case LoopEventIndex::ON_FIXED_UPDATE: 
					CPPEntityContext::bind_time_delta((float)((double)(Configuration::get_instance()->performance_settings.fixed_frame_time) / MS_IN_S));
					script_instance->on_fixed_update(); 
					break;
				case LoopEventIndex::ON_LATE_UPDATE: script_instance->on_late_update(); break;
				case LoopEventIndex::ON_ENTITY_FINISH: script_instance->on_entity_finish(); break;
				case LoopEventIndex::ON_OUTPUT: script_instance->on_output(); break;
			}
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
	for (auto& script_collection : sceneLayer->get_components_by_type<ScriptCollection>())
		bind_and_call(script_collection, sceneLayer->get_id(), script_collection->get_entity_id(), methodName);
}

void Omnific::CPPScriptingSystem::finalize()
{
	this->is_initialized = false;
}