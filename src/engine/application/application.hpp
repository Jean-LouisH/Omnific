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

#include <stdint.h>
#include "scene/scene.hpp"
#include "os/os.hpp"
#include "os/file_access.hpp"
#include "os/profiler.hpp"
#include "application/scripting/scripting_apis/scripting_apis.hpp"
#include "scene_serializer.hpp"
#include "configuration.hpp"
#include "command_line.hpp"
#include "scene_storage.hpp"
#include "scripting/python_vm/python_vm.hpp"
#include "scripting/scripting.hpp"
#include <vector>
#include <stack>
#include <string>
#include <memory>

namespace Omnia
{
	class Application
	{
	public:
		Application();
		void initialize();
		void executeOnStartMethods();
		void executeOnInputMethods();
		void executeOnLogicFrameMethods();
		void executeOnComputeFrameMethods();
		void executeOnOutputMethods();
		void executeOnFinishMethods();
		void deinitialize();
		Scene& getActiveScene();
		Configuration& getConfiguration();
	private:
		std::shared_ptr<Configuration> configuration;
		std::shared_ptr<SceneSerializer> sceneSerializer;
		std::shared_ptr<CommandLine> commandLine;
		std::shared_ptr<Scripting> scripting;
		std::shared_ptr<SceneStorage> sceneStorage;
	};
}