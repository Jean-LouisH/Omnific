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

#include "python_scripting_system.hpp"
#include "python_entity_context.hpp"
#include <core/singletons/scene_storage.hpp>
#include <core/singletons/os/os.hpp>
#include <iostream>

#include "embedded_module.hpp"
#include <iostream>
#include <set>


void Omnia::PythonScriptingSystem::initialize()
{
	Logger& logger = OS::get_logger();
	logger.write("Initializing Python Scripting System...");
	pybind11::initialize_interpreter();
	this->is_vmstarted = true;
}

void Omnia::PythonScriptingSystem::execute_command(std::string command)
{
	pybind11::exec("from omnia import *");
	pybind11::exec(command);
}

void Omnia::PythonScriptingSystem::load_script_modules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		if (!this->has_modules_loaded_on_this_update)
		{
			this->python_script_instances.clear();

			pybind11::module_ sys = pybind11::module_::import("sys");
			pybind11::object path = sys.attr("path");
			std::set<std::string> added_paths;

			for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			{
				for (std::shared_ptr<ScriptCollection> script_collection : it.second->get_components_by_type<ScriptCollection>())
				{
					for (std::shared_ptr<Script> script : script_collection->scripts)
					{
						std::string script_filepath = script->get_name();

						if (script->get_language_name() == "Python")
						{
							try
							{
								if (added_paths.count(script_filepath) == 0)
								{
									std::string new_path = OS::get_file_access().get_executable_directory_path() +
										"//data//" + OS::get_file_access().get_path_before_file(script_filepath);
#ifdef _DEBUG
									new_path = OS::get_file_access().get_executable_directory_path();
									new_path = new_path.substr(0, new_path.find("out\\build\\x64-Debug\\src\\main"));
									std::string data_folder = OS::get_file_access().get_data_directory_path();
									data_folder = data_folder.substr(data_folder.find("data/"), data_folder.size() - 1);
									new_path += data_folder + OS::get_file_access().get_path_before_file(script_filepath);
#endif
									pybind11::str new_path_obj = pybind11::str(new_path);
									new_path_obj = new_path_obj.attr("replace")("//", "/");
									new_path_obj = new_path_obj.attr("replace")("/", "\\");
									new_path = new_path_obj.cast<std::string>();

									path.attr("insert")(0, new_path);
									added_paths.emplace(new_path);
								}

								std::string module_name = OS::get_file_access().get_file_name_without_extension(script_filepath);
								pybind11::module_ new_pybind11_module = pybind11::module_::import(module_name.c_str());

								PythonScriptInstance script_instance;
								std::vector<std::string> method_names = {
									"on_start",
									"on_input",
									"on_early",
									"on_logic",
									"on_compute",
									"on_late",
									"on_output",
									"on_finish"
								};
								script_instance.set_data(new_pybind11_module.attr("omnia_script")());

								for (int i = 0; i < method_names.size(); i++)
								{
									try
									{
										std::string method_name = method_names.at(i);
										pybind11::object test = script_instance.test(method_name.c_str());
										script_instance.set_callable(method_name);
									}
									catch (const pybind11::error_already_set& e) //using the exception catch to detect if method is not callable
									{

									}
								}

								this->python_script_instances.emplace(script_filepath + std::to_string(script_collection->get_entity_id()), script_instance);
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
}

void Omnia::PythonScriptingSystem::on_start(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

#ifdef DEBUG_CONSOLE_ENABLED
	if (OS::get_input().has_requested_command_line())
	{
		std::string command;

		OS::get_window().hide();
		std::cout << std::endl << ">";
		std::cin.ignore(1, '\n');
		std::getline(std::cin, command);
		this->execute_command(command);
		OS::get_window().show();
	}
#endif

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_queued_methods(it.second->get_start_entity_queue(), it.second, "on_start");
}

void Omnia::PythonScriptingSystem::on_input(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_update_methods(it.second, "on_input");
}

void Omnia::PythonScriptingSystem::on_early(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_update_methods(it.second, "on_early");
}

void Omnia::PythonScriptingSystem::on_logic(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_update_methods(it.second, "on_logic");
}

void Omnia::PythonScriptingSystem::on_compute(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_update_methods(it.second, "on_compute");
}

void Omnia::PythonScriptingSystem::on_late(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_update_methods(it.second, "on_late");
}

void Omnia::PythonScriptingSystem::on_finish(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		for (auto it : PythonEntityContext::get_scene()->get_scene_layers())
			this->execute_queued_methods(it.second->get_finish_entity_queue(), it.second, "on_finish");

	this->has_modules_loaded_on_this_update = false;
}

void Omnia::PythonScriptingSystem::finalize()
{
	if (this->is_vmstarted)
		pybind11::finalize_interpreter();
}

void Omnia::PythonScriptingSystem::execute_queued_methods(
	std::queue<EntityID> entity_queue,
	std::shared_ptr<SceneLayer> scene_layer,
	const char* method_name)
{
	while (!entity_queue.empty())
	{
		std::shared_ptr<Entity> entity = scene_layer->get_entity(entity_queue.front());
		std::shared_ptr<ScriptCollection> script_collection = scene_layer->get_component_by_type<ScriptCollection>(entity->get_id());
		if (script_collection != nullptr)
		{
			this->bind_and_call(script_collection, scene_layer->get_id(), script_collection->get_entity_id(), method_name);
		}
		entity_queue.pop();
	}
}

void Omnia::PythonScriptingSystem::execute_update_methods(std::shared_ptr<SceneLayer> scene_layer, const char* method_name)
{
	std::vector<std::shared_ptr<ScriptCollection>> script_collections = scene_layer->get_components_by_type<ScriptCollection>();
	size_t script_collections_count = script_collections.size();

	for (size_t i = 0; i < script_collections_count; i++)
	{
		std::shared_ptr<ScriptCollection> script_collection = script_collections.at(i);
		this->bind_and_call(script_collection, scene_layer->get_id(), script_collection->get_entity_id(), method_name);
	}
}

void Omnia::PythonScriptingSystem::bind_and_call(
	std::shared_ptr<ScriptCollection> script_collection,
	SceneLayerID scene_layer_id,
	EntityID entity_id,
	const char* method_name)
{
	for (size_t j = 0; j < script_collection->scripts.size(); j++)
	{
		std::string script_path = script_collection->scripts.at(j)->get_name() + std::to_string(entity_id);

		if (this->python_script_instances.count(script_path))
		{
			if (this->python_script_instances.at(script_path).has_callable(method_name))
			{
				PythonEntityContext::bind_entity(
					scene_layer_id,
					entity_id);

				try
				{
					this->python_script_instances.at(script_path).call(method_name);
				}
				catch (const pybind11::error_already_set& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
	}
}