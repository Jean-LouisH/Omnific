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
#include "application/scene/scene.hpp"
#include <application/scene_storage.hpp>
#include "utilities/aliases.hpp"
#include "scripting_language.hpp"
#include <memory>
#include <vector>

namespace Omnia
{
	class Scripting
	{
	public:
		Scripting();
		void onModifiedScriptInstance(Scene scene);
		void executeOnStartMethods(Scene& scene);
		void executeOnInputMethods(Scene& scene);
		void executeOnLogicFrameMethods(Scene& scene);
		void executeOnComputeFrameMethods(Scene& scene);
		void executeOnOutputMethods(Scene& scene);
		void executeOnFinishMethods(Scene& scene);
		void setSceneStorage(SceneStorage* sceneStorage);
	private:
		std::vector<std::unique_ptr<ScriptingLanguage>> scriptingLanguages;

		std::unique_ptr<PythonVM> pythonVM;
		std::unique_ptr<CPPNative> cppNative;
	};
}

