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

#pragma once

#define _STL_CRT_SECURE_INVALID_PARAMETER(expr) _CRT_SECURE_INVALID_PARAMETER(expr)

#include "scene/scene.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include "foundations/aliases.hpp"
#include "systems/system.hpp"
#include <scene/components/script_collection.hpp>
#include <memory>
#include <vector>

#include <unordered_map>
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "python_script_instance.hpp"

namespace Omnific
{
	class PythonScriptingSystem : public System
	{
	public:
		PythonScriptingSystem()
		{
			this->type = TYPE_STRING;
		};

		static constexpr const char* TYPE_STRING = "PythonScriptingSystem";

		virtual Registerable* instance() override
		{
			return new PythonScriptingSystem(*this);
		}

		virtual void initialize() override;
		virtual void finalize() override;
		virtual void on_input(std::shared_ptr<Scene> scene) override;
		virtual void on_entity_start(std::shared_ptr<Scene> scene) override;
		virtual void on_early_update(std::shared_ptr<Scene> scene) override;
		virtual void on_update(std::shared_ptr<Scene> scene) override;
		virtual void on_fixed_update(std::shared_ptr<Scene> scene) override;
		virtual void on_late_update(std::shared_ptr<Scene> scene) override;
		virtual void on_entity_finish(std::shared_ptr<Scene> scene) override;
		virtual void on_output(std::shared_ptr<Scene> scene) override;

		void execute_command(std::string command);
		void load_script_modules(std::shared_ptr<Scene> scene);
	private:
		bool is_vmstarted = false;
		bool has_modules_loaded_on_this_update = false;
		std::unordered_map<std::string, PythonScriptInstance> python_script_instances;
		std::unordered_map<std::string, std::vector<std::string>> methods_with_instances;

		void execute_queued_methods(
			std::queue<EntityID> entity_queue,
			std::shared_ptr<SceneLayer> scene_layer,
			const char* method_name);
		void execute_update_methods(
			std::shared_ptr<SceneLayer> scene_layer,
			const char* method_name);
		void bind_and_call(std::shared_ptr<ScriptCollection> script_collection,
			SceneLayerID scene_layer_id,
			EntityID entity_id,
			std::string method_name);
	};
}

#endif /*ENABLE_PYTHON_BUILD*/