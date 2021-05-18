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

#include "script_call_batch.hpp"
#include "utilities/collections/map.hpp"
#include "game/scene/script.hpp"
#include "scripting_apis/scripting_apis.hpp"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"

namespace Lilliputian
{
	class VirtualMachine
	{
	public:
		VirtualMachine(
			Vector<String>* scripts,
			ScriptingAPIs* scriptingAPIs);
		~VirtualMachine();
		void loadCurrentSceneScriptModules();
		void executeOnStartMethods(Vector<ScriptCallBatch> scriptCallBatches);
		void executeOnInputMethods(Vector<ScriptCallBatch> scriptCallBatches);
		void executeOnFrameMethods(Vector<ScriptCallBatch> scriptCallBatches);
		void executeOnComputeMethods(Vector<ScriptCallBatch> scriptCallBatches, uint32_t msPerComputeUpdate);
		void executeOnLateMethods(Vector<ScriptCallBatch> scriptCallBatches);
		void executeOnFinalMethods(Vector<ScriptCallBatch> scriptCallBatches);
	private:
		Vector<String>* scripts;
		ScriptingAPIs* scriptingAPIs;
		pybind11::scoped_interpreter guard{};
		Map<String, pybind11::module_> modules;

		void executeMethods(Vector<ScriptCallBatch> scriptCallBatches, const char* methodName);
	};
}