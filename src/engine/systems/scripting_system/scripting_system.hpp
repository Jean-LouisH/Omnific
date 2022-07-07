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

#include "python_vm/python_vm.hpp"
#include "cpp_native/cpp_native.hpp"
#include "scene/scene.hpp"
#include <scene_storage.hpp>
#include <scene_serializer.hpp>
#include "utilities/aliases.hpp"
#include "scripting_language.hpp"
#include "update_system.hpp"
#include <memory>
#include <vector>

namespace Omnia
{
	class ScriptingSystem : public UpdateSystem
	{
	public:
		ScriptingSystem();
		virtual void initialize() override;
		void process(std::shared_ptr<Scene> scene) override;
		virtual void deinitialize() override;
		void executeCommand(std::string command);
		void loadScriptModules(std::shared_ptr<Scene> scene);
		void executeOnStartMethods(std::shared_ptr<Scene> scene);
		void executeOnInputMethods(std::shared_ptr<Scene> scene);
		void executeOnLogicFrameMethods(std::shared_ptr<Scene> scene);
		void executeOnComputeFrameMethods(std::shared_ptr<Scene> scene);
		void executeOnOutputMethods(std::shared_ptr<Scene> scene);
		void executeOnFinishMethods(std::shared_ptr<Scene> scene);
		void setSceneSerializer(std::shared_ptr<SceneSerializer> sceneSerializer);
		void setSceneStorage(std::shared_ptr<SceneStorage> sceneStorage);
	private:
		std::vector<std::unique_ptr<ScriptingLanguage>> scriptingLanguages;

		std::unique_ptr<PythonVM> pythonVM;
		std::unique_ptr<CPPNative> cppNative;
	};
}

