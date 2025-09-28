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

#ifdef ENABLE_PYTHON_BUILD

#include "python_scripting_system.hpp"
#include "python_entity_context.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <iostream>

#include "embedded_module.hpp"
#include <SDL_platform.h>
#include <set>
#include <foundations/singletons/profiler.hpp>

#define PYTHON_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_input_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_entity_start_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_early_update_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_update_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_fixed_update_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_late_update_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_entity_finish_frame_time"
#define PYTHON_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "python_scripting_system_on_output_frame_time"


void Omnific::PythonScriptingSystem::initialize()
{
	this->is_initialized = true;
	FileAccess& file_access = Platform::get_file_access();
	Logger& logger = Platform::get_logger();
	logger.write("Initializing Python Scripting System...");
	std::string python_home_path = file_access.get_executable_directory_path();
    std::wstring python_home_wstring(python_home_path.begin(), python_home_path.end());

	PyConfig config;
	PyConfig_InitIsolatedConfig(&config);
	PyConfig_SetBytesString(&config, &config.program_name, (file_access.get_executable_name()).c_str());
	PyConfig_SetBytesString(&config, &config.home, python_home_path.c_str());
	PyWideStringList_Append(&config.module_search_paths, python_home_wstring.c_str());
	config.isolated = 1;          
    config.use_environment = 0;
    config.site_import = 1; 
	Py_InitializeFromConfig(&config);
	PyConfig_Clear(&config);
	pybind11::exec(R"(
        import sys
        print("Python home set to:", sys.prefix)
        print("Python sys.path set to:", sys.path)
    )");
	pybind11::module_ sys = pybind11::module_::import("sys");
	pybind11::object path = sys.attr("path");
	pybind11::object version_info = sys.attr("version_info");
	pybind11::str version_info_major = pybind11::str(version_info.attr("major"));
	pybind11::str version_info_minor = pybind11::str(version_info.attr("minor"));
	this->is_vmstarted = true;
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_input_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_entity_start_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_early_update_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_update_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_fixed_update_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_late_update_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_entity_finish_frame_time"});
	Profiler::add_clock(PYTHON_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"python_scripting_system", "on_output_frame_time"});
}

void Omnific::PythonScriptingSystem::execute_command(std::string command)
{
	pybind11::exec("from omnific import *");
	pybind11::exec(command);
}

void Omnific::PythonScriptingSystem::load_script_modules()
{
	if (!this->has_modules_loaded_on_this_update && 
		EventBus::has_event_with_parameter_key(OMNIFIC_EVENT_COMPONENT_ADDED, ScriptCollection::TYPE_STRING))
	{
		if (EventBus::has_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED))
		{
			this->python_script_instances.clear();
			this->last_modified_times_for_script_files.clear();
			this->instances_with_methods.clear();
		}

		pybind11::module_ sys = pybind11::module_::import("sys");
		pybind11::object path = sys.attr("path");
		pybind11::object version_info = sys.attr("version_info");
		pybind11::str version_info_major = pybind11::str(version_info.attr("major"));
		pybind11::str version_info_minor = pybind11::str(version_info.attr("minor"));
		std::set<std::string> added_paths;

		for (std::shared_ptr<Component> component : EventBus::get_components(OMNIFIC_EVENT_COMPONENT_ADDED, ScriptCollection::TYPE_STRING))
		{
			std::shared_ptr<ScriptCollection> script_collection = std::dynamic_pointer_cast<ScriptCollection>(component);
			
			if (script_collection != nullptr)
			{
				for (std::shared_ptr<Script>& script : script_collection->scripts)
				{
					std::string script_filepath = script->get_name();

					if (script->get_language_name() == "Python")
					{
						try
						{
							if (added_paths.count(script_filepath) == 0)
							{
								FileAccess& file_access = Platform::get_file_access();
								std::string new_path = file_access.get_path_before_file(
									file_access.find_path(script_filepath));
								std::string full_script_filepath = file_access.find_path(script_filepath);
	#ifdef WIN32
								pybind11::str new_path_obj = pybind11::str(new_path);
								new_path_obj = new_path_obj.attr("replace")("//", "/");
								new_path_obj = new_path_obj.attr("replace")("/", "\\");
								new_path = new_path_obj.cast<std::string>();
	#endif
								path.attr("insert")(0, new_path);
								added_paths.emplace(new_path);

								this->last_modified_times_for_script_files.emplace(
									full_script_filepath, 
									Platform::get_file_access().get_last_modified_time(full_script_filepath)
								);
							}

							std::string module_name = Platform::get_file_access().get_file_name_without_extension(script_filepath);
							pybind11::module_ importlib = pybind11::module_::import("importlib");
							pybind11::module_ module = pybind11::module_::import(module_name.c_str());
							pybind11::object reloaded_module = importlib.attr("reload")(module);

							PythonScriptInstance script_instance;
							std::vector<std::string> method_names = {
								"on_input",
								"on_entity_start",
								"on_early_update",
								"on_update",
								"on_fixed_update",
								"on_late_update",
								"on_entity_finish",
								"on_output"
							};
							script_instance.set_data(reloaded_module.attr("omnific_script")());
							std::string script_instance_name = script_filepath + std::to_string(script_collection->get_entity_id());
							this->script_collections_with_instances.emplace(script_instance_name, script_collection);

							for (int i = 0; i < method_names.size(); ++i)
							{
								try
								{
									std::string method_name = method_names.at(i);
									pybind11::object test = script_instance.test(method_name.c_str());
									script_instance.set_callable(method_name);
									if (this->instances_with_methods.count(method_name))
									{
										this->instances_with_methods.at(method_name).push_back(script_instance_name);
									}
									else
									{
										std::vector<std::string> script_instance_names;
										script_instance_names.push_back(script_instance_name);
										this->instances_with_methods.emplace(method_name, script_instance_names);
									}
								}
								catch (const pybind11::error_already_set& e) //using the exception catch to detect if method is not callable
								{

								}
							}
							this->python_script_instances.emplace(script_instance_name, script_instance);
						}
						catch (const pybind11::error_already_set& e)
						{
							std::cout << e.what() << std::endl;
						}
					}
				}
			}
		}
		this->has_modules_loaded_on_this_update = true;
	}
}

void Omnific::PythonScriptingSystem::on_entity_start()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_START_FRAME_TIME_CLOCK_NAME);
	std::string method_name = "on_entity_start";
	frame_time_clock->set_start();

	if (this->have_components_changed() || this->has_any_script_been_modified())
		this->load_script_modules();

#ifdef DEBUG_CONSOLE_ENABLED
	if (Platform::get_inputs().has_requested_command_line())
	{
		std::string command;

		Platform::get_window().hide();
		std::cout << std::endl << ">";
		std::cin.ignore(1, '\n');
		std::getline(std::cin, command);
		this->execute_command(command);
		Platform::get_window().show();
	}
#endif


	if (scene != nullptr && this->instances_with_methods.count(method_name))
		this->execute_queued_methods(scene->get_start_entity_queue(), scene, method_name.c_str());

	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_input()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_input");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_early_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_early_update");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_update");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_fixed_update");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_late_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_LATE_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_late_update");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_entity_finish()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_ENTITY_FINISH_FRAME_TIME_CLOCK_NAME);
	std::string method_name = "on_entity_finish";
	frame_time_clock->set_start();

	if (this->have_components_changed() || this->has_any_script_been_modified())
		this->load_script_modules();

	if (scene != nullptr && this->instances_with_methods.count(method_name))
		this->execute_queued_methods(scene->get_finish_entity_queue(), scene, method_name.c_str());

	this->has_modules_loaded_on_this_update = false;
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::on_output()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PYTHON_SCRIPTING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->execute_regular_methods(scene, "on_output");
	frame_time_clock->set_end();
}

void Omnific::PythonScriptingSystem::finalize()
{
	if (this->is_vmstarted)
	{
		Py_Finalize();
		//pybind11::finalize_interpreter();
	}

	this->is_initialized = false;
}

void Omnific::PythonScriptingSystem::execute_queued_methods(
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

					if (this->python_script_instances.count(instance_name))
					{
						if (this->python_script_instances.at(instance_name).has_callable(method_name))
						{
							this->bind_and_call(script_collection, instance_name, method_name);
						}
					}
				}
		}
		entity_queue.pop();
	}
}

void Omnific::PythonScriptingSystem::execute_regular_methods(std::shared_ptr<Scene> scene, const char* method_name)
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

void Omnific::PythonScriptingSystem::bind_and_call(
	std::shared_ptr<ScriptCollection> script_collection,
	std::string instance_name,
	std::string method_name)
{
	PythonEntityContext::bind_entity(script_collection->get_entity_id());

	if (method_name != "on_fixed_update")
	{
		std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(FRAME_TIME_CLOCK_NAME);
		frame_time_clock->set_end();
		PythonEntityContext::bind_time_delta(frame_time_clock->get_delta_in_seconds());
	}
	else
	{
		PythonEntityContext::bind_time_delta((float)((double)(Configuration::get_instance()->performance_settings.fixed_frame_time) / MS_IN_S));
	}

	try
	{
		this->python_script_instances.at(instance_name).call(method_name);
	}
	catch (const pybind11::error_already_set& e)
	{
		std::cout << e.what() << std::endl;
	}
}

bool Omnific::PythonScriptingSystem::has_any_script_been_modified()
{
	bool has_been_modified = false;
	const int script_check_update_time = 1;

	this->script_modified_time_clock->set_end();

	if (this->script_modified_time_clock->get_delta() >= script_check_update_time * MS_IN_S)
	{
		this->script_modified_time_clock->set_start();
		for (auto& [script_filepath, last_modified_time] : this->last_modified_times_for_script_files)
		{
			if (last_modified_time != Platform::get_file_access().get_last_modified_time(script_filepath))
			{
				has_been_modified = true;
			}
		}
	}

	return has_been_modified;
}

#endif /*ENABLE_PYTHON_BUILD*/
