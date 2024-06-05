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
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <iostream>

typedef void(ScriptProcedure)(void);

void Omnia::CPPScriptingSystem::initialize()
{
	Logger& logger = Platform::get_logger();
	logger.write("Initializing C++ Scripting System...");
}

void Omnia::CPPScriptingSystem::load_script_modules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		this->dynamic_library_handle = Platform::get_shared_library_access().open(this->native_assembly_filename);
		this->execute("load_script_instances");
	}
}

void Omnia::CPPScriptingSystem::on_start(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_start");
}

void Omnia::CPPScriptingSystem::on_input(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_input");
}

void Omnia::CPPScriptingSystem::on_early(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_early");
}

void Omnia::CPPScriptingSystem::on_logic(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_logic");
}

void Omnia::CPPScriptingSystem::on_compute(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_compute");
}

void Omnia::CPPScriptingSystem::on_late(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_late");
}

void Omnia::CPPScriptingSystem::on_finish(std::shared_ptr<Scene> scene)
{
	if (this->has_scene_changed(scene))
		this->load_script_modules(scene);

	if (scene != nullptr)
		this->execute("on_finish");
}

void Omnia::CPPScriptingSystem::execute(std::string method_name)
{
	if (this->dynamic_library_handle != nullptr)
	{
		ScriptProcedure* procedure =
			(ScriptProcedure*)Platform::get_shared_library_access().get_procedure(this->dynamic_library_handle, method_name.c_str());

		if (procedure != nullptr)
			procedure();
	}
}

void Omnia::CPPScriptingSystem::finalize()
{
	Platform::get_shared_library_access().close(this->dynamic_library_handle);
}