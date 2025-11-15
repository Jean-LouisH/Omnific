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

#include "imperium_scripting_system.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <iostream>

#include <SDL_platform.h>
#include <set>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/event_bus.hpp>

#define IMPERIUM_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_input_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_entity_start_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_early_update_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_update_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_fixed_update_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_late_update_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_entity_finish_frame_time"
#define IMPERIUM_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "imperium_scripting_system_on_output_frame_time"


void Omnific::ImperiumScriptingSystem::initialize()
{
	this->is_initialized = true;
	this->is_vmstarted = true;
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_input_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_entity_start_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_early_update_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_update_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_fixed_update_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_late_update_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_entity_finish_frame_time"});
	Profiler::add_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"imperium_scripting_system", "on_output_frame_time"});
}

void Omnific::ImperiumScriptingSystem::load_script_modules()
{
	if (!this->has_modules_loaded_on_this_frame && 
		EventBus::has_event_with_parameter_key(OMNIFIC_EVENT_COMPONENT_ADDED, ScriptCollection::TYPE_STRING))
	{
		if (EventBus::has_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED))
		{
			//this->imperium_script_instances.clear();
			this->last_modified_times_for_script_files.clear();
			this->instances_with_methods.clear();
		}

		for (std::shared_ptr<Component> component : EventBus::get_components(OMNIFIC_EVENT_COMPONENT_ADDED, ScriptCollection::TYPE_STRING))
		{
			std::shared_ptr<ScriptCollection> script_collection = std::dynamic_pointer_cast<ScriptCollection>(component);
			
			if (script_collection != nullptr)
			{
				for (std::shared_ptr<Script>& script : script_collection->scripts)
				{
					std::string script_filepath = script->get_name();

					if (script->get_language_name() == "Imperium")
					{

					}
				}
			}
		}
		this->has_modules_loaded_on_this_frame = true;
	}
}

void Omnific::ImperiumScriptingSystem::on_entity_start()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME);
	std::string method_name = "on_entity_start";
	frame_time_clock->set_start();

	if (this->have_components_changed() || this->has_any_script_been_modified())
		this->load_script_modules();

	if (scene != nullptr && this->instances_with_methods.count(method_name))
		this->execute_queued_methods(scene->get_start_entity_queue(), scene, method_name.c_str());

	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_input()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_input");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_early_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_early_update");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_update");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_fixed_update");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_late_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_late_update");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_entity_finish()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME);
	std::string method_name = "on_entity_finish";
	frame_time_clock->set_start();

	if (this->have_components_changed() || this->has_any_script_been_modified())
		this->load_script_modules();

	if (scene != nullptr && this->instances_with_methods.count(method_name))
		this->execute_queued_methods(scene->get_finish_entity_queue(), scene, method_name.c_str());

	this->has_modules_loaded_on_this_frame = false;
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::on_output()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(IMPERIUM_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_output");
	frame_time_clock->set_end();
}

void Omnific::ImperiumScriptingSystem::finalize()
{
	if (this->is_vmstarted)
	{

	}

	this->is_initialized = false;
}

void Omnific::ImperiumScriptingSystem::execute_queued_methods(
	std::queue<EntityID> entity_queue,
	std::shared_ptr<Scene> scene,
	const char* method_name)
{
	while (!entity_queue.empty())
	{
		std::shared_ptr<Entity> entity = scene->get_entity(entity_queue.front());
		std::shared_ptr<ScriptCollection> script_collection = scene->get_component_by_type<ScriptCollection>(entity->get_id());
		if (script_collection != nullptr)
		{
				for (size_t j = 0; j < script_collection->scripts.size(); ++j)
				{
					std::string instance_name = script_collection->scripts.at(j)->get_name() + std::to_string(script_collection->get_entity_id());

					// if (this->imperium_script_instances.count(instance_name))
					// {
					// 	if (this->imperium_script_instances.at(instance_name).has_callable(method_name))
					// 	{
					// 		this->bind_and_call(script_collection, instance_name, method_name);
					// 	}
					// }
				}
		}
		entity_queue.pop();
	}
}

void Omnific::ImperiumScriptingSystem::execute_regular_methods(std::shared_ptr<Scene> scene, const char* method_name)
{
	if (this->have_components_changed() || this->has_any_script_been_modified())
		this->load_script_modules();

	if (scene != nullptr && this->instances_with_methods.count(method_name))
	{
		std::vector<std::string> instance_names = this->instances_with_methods.at(method_name);
		std::string* instance_names_ptr = instance_names.data();
		int instance_name_count = instance_names.size();

		for (int i = 0; i < instance_name_count; i++)
		{
			std::string instance_name = instance_names_ptr[i];
			std::shared_ptr<ScriptCollection> script_collection = this->script_collections_with_instances.at(instance_name);
			this->bind_and_call(script_collection, instance_name, method_name);
		}
	}
}

void Omnific::ImperiumScriptingSystem::bind_and_call(
	std::shared_ptr<ScriptCollection> script_collection,
	std::string instance_name,
	std::string method_name)
{
	//ImperiumEntityContext::bind_entity(script_collection->get_entity_id());

	if (method_name != "on_fixed_update")
	{
		std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(FRAME_TIME_CLOCK_NAME);
		frame_time_clock->set_end();
		//ImperiumEntityContext::bind_time_delta(frame_time_clock->get_delta_in_seconds());
	}
	else
	{
		//ImperiumEntityContext::bind_time_delta((float)((double)(Configuration::get_instance()->performance_settings.fixed_frame_time) / MS_IN_S));
	}
}

bool Omnific::ImperiumScriptingSystem::has_any_script_been_modified()
{
	bool has_been_modified = false;
	const int script_check_update_time = 1;

	// this->script_modified_time_clock->set_end();

	// if (this->script_modified_time_clock->get_delta() >= script_check_update_time * MS_IN_S)
	// {
	// 	this->script_modified_time_clock->set_start();
	// 	for (auto& [script_filepath, last_modified_time] : this->last_modified_times_for_script_files)
	// 	{
	// 		if (last_modified_time != Platform::get_file_access().get_last_modified_time(script_filepath))
	// 		{
	// 			has_been_modified = true;
	// 		}
	// 	}
	// }

	return has_been_modified;
}

