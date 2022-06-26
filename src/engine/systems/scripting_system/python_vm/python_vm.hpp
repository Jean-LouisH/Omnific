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

#define _STL_CRT_SECURE_INVALID_PARAMETER(expr) _CRT_SECURE_INVALID_PARAMETER(expr)

#include <unordered_map>
#include "../vm_scripting_language.hpp"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "module.hpp"
#include <scene/scene.hpp>
#include <memory>

namespace Omnia
{
	class PythonVM : VMScriptingLanguage
	{
	public:
		PythonVM();
		~PythonVM();
		void initialize();
		void loadScriptModules(std::shared_ptr<Scene> scene);
		void executeOnStartMethods(std::shared_ptr<SceneTree> sceneTree);
		void executeOnInputMethods(std::shared_ptr<SceneTree> sceneTree);
		void executeOnLogicFrameMethods(std::shared_ptr<SceneTree> sceneTree);
		void executeOnComputeFrameMethods(std::shared_ptr<SceneTree> sceneTree);
		void executeOnOutputMethods(std::shared_ptr<SceneTree> sceneTree);
		void executeOnFinishMethods(std::shared_ptr<SceneTree> sceneTree);
		void deinitialize();
	private:
		std::unordered_map<std::string, Module> modules;

		void executeQueuedMethods(
			std::queue<EntityID>& entityQueue, 
			std::shared_ptr<SceneTree> sceneTree, 
			const char* methodName);
		void executeUpdateMethods(
			std::shared_ptr<SceneTree> sceneTree, 
			const char* methodName);
		void bindAndCall(std::shared_ptr<ScriptCollection> scriptCollection,
			SceneTreeID sceneTreeID,
			EntityID entityID,
			const char* methodName);
	};
}