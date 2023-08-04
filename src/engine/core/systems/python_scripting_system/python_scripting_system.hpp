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

#include "core/scene/scene.hpp"
#include <core/singletons/scene_storage.hpp>
#include "core/utilities/aliases.hpp"
#include "core/system.hpp"
#include <core/components/script_collection.hpp>
#include <memory>
#include <vector>

#include <unordered_map>
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "python_script_instance.hpp"

namespace Omnia
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
		virtual void onStart(std::shared_ptr<Scene> scene) override;
		virtual void onInput(std::shared_ptr<Scene> scene) override;
		virtual void onEarly(std::shared_ptr<Scene> scene) override;
		virtual void onLogic(std::shared_ptr<Scene> scene) override;
		virtual void onCompute(std::shared_ptr<Scene> scene) override;
		virtual void onLate(std::shared_ptr<Scene> scene) override;
		virtual void onFinish(std::shared_ptr<Scene> scene) override;

		void executeCommand(std::string command);
		void loadScriptModules(std::shared_ptr<Scene> scene);
	private:
		bool isVMStarted = false;
		bool hasModulesLoadedOnThisUpdate = false;
		std::unordered_map<std::string, PythonScriptInstance> pythonScriptInstances;

		void executeQueuedMethods(
			std::queue<EntityID> entityQueue,
			std::shared_ptr<SceneLayer> sceneLayer,
			const char* methodName);
		void executeUpdateMethods(
			std::shared_ptr<SceneLayer> sceneLayer,
			const char* methodName);
		void bindAndCall(std::shared_ptr<ScriptCollection> scriptCollection,
			SceneLayerID sceneLayerID,
			EntityID entityID,
			const char* methodName);
	};
}

