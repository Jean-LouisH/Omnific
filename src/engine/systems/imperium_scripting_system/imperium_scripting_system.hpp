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

#pragma once

#include "scene/scene.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include "foundations/aliases.hpp"
#include "systems/system.hpp"
#include <scene/components/script_collection.hpp>
#include <memory>
#include <vector>
#include <foundations/singletons/platform/platform.hpp>

namespace Omnific
{
	/* Custom scripting system as a middle ground between 
	   Python and C++ advantages. */
	class ImperiumScriptingSystem : public System
	{
	public:
		ImperiumScriptingSystem()
		{
			this->type = TYPE_STRING;
		};

		static constexpr const char* TYPE_STRING = "ImperiumScriptingSystem";

		virtual Registerable* instance() override
		{
			return new ImperiumScriptingSystem(*this);
		}

		virtual void initialize() override;
		virtual void finalize() override;
		virtual void on_input() override;
		virtual void on_entity_start() override;
		virtual void on_early_update() override;
		virtual void on_update() override;
		virtual void on_fixed_update() override;
		virtual void on_late_update() override;
		virtual void on_entity_finish() override;
		virtual void on_output() override;

		void execute_command(std::string command);
		void load_script_modules();
	private:
		bool is_vmstarted = false;
		bool has_modules_loaded_on_this_frame = false;
		//std::unordered_map<std::string, ImperiumScriptInstance> imperium_script_instances;
		std::unordered_map<std::string, std::string> last_modified_times_for_script_files;
		std::unordered_map<std::string, std::vector<std::string>> instances_with_methods;
		std::unordered_map<std::string, std::shared_ptr<ScriptCollection>> script_collections_with_instances;
		std::shared_ptr<Clock> script_modified_time_clock;

		void execute_queued_methods(
			std::queue<EntityID> entity_queue,
			std::shared_ptr<Scene> scene,
			const char* method_name);
		void execute_regular_methods(std::shared_ptr<Scene> scene, const char* method_name);
		void bind_and_call(std::shared_ptr<ScriptCollection> script_collection,
			std::string instance_name,
			std::string method_name);

		bool has_any_script_been_modified();
	};
}

