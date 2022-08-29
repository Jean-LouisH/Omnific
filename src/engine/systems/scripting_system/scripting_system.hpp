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

#include "scripting_languages/python_vm/python_vm.hpp"
#include "scripting_languages/cpp_native/cpp_native.hpp"
#include "scene/scene.hpp"
#include <scene_storage.hpp>
#include <scene_serializer.hpp>
#include "utilities/aliases.hpp"
#include "scripting_languages/scripting_language.hpp"
#include "systems/system.hpp"
#include <memory>
#include <vector>

namespace Omnia
{
	class ScriptingSystem : public System
	{
	public:
		ScriptingSystem()
		{
			this->scriptingLanguages.emplace("PythonVM", std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<PythonVM>(new PythonVM())));
			this->scriptingLanguages.emplace("CPPNative", std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<CPPNative>(new CPPNative())));
			this->type = TYPE_STRING;
			this->threadType = ThreadType::UPDATE;
		};

		static constexpr const char* TYPE_STRING = "ScriptingSystem";

		virtual void initialize() override;
		virtual void finalize() override;
		virtual void onStart(std::shared_ptr<Scene> scene) override;
		virtual void onInput(std::shared_ptr<Scene> scene) override;
		virtual void onLogic(std::shared_ptr<Scene> scene) override;
		virtual void onCompute(std::shared_ptr<Scene> scene) override;
		virtual void onOutput(std::shared_ptr<Scene> scene) override;
		virtual void onFinish(std::shared_ptr<Scene> scene) override;

		void executeCommand(std::string command);
		void loadScriptModules(std::shared_ptr<Scene> scene);
	private:
		std::unordered_map<std::string, std::shared_ptr<ScriptingLanguage>> scriptingLanguages;
	};
}

