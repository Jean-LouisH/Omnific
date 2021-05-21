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

#include "virtual_machine.hpp"
#include "embedded_module.hpp"
#include <iostream>

Lilliputian::VirtualMachine::VirtualMachine(
	Vector<String>* scripts)
{
	this->scripts = scripts;
}

Lilliputian::VirtualMachine::~VirtualMachine()
{

}

void Lilliputian::VirtualMachine::loadCurrentSceneScriptModules()
{
	for (int i = 0; i < this->scripts->size(); i++)
	{
		try
		{
			String moduleName = this->scripts->at(i);
			pybind11::module_ newModule = pybind11::module_::import(moduleName.c_str());
			this->modules.emplace(moduleName, newModule);
		}
		catch (const pybind11::error_already_set& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Lilliputian::VirtualMachine::executeOnStartMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_start");
}

void Lilliputian::VirtualMachine::executeOnInputMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_input");
}

void Lilliputian::VirtualMachine::executeOnFrameMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_frame");
}

void Lilliputian::VirtualMachine::executeOnComputeMethods(Vector<ScriptCallBatch> scriptCallBatches, uint32_t msPerComputeUpdate)
{
	this->executeMethods(scriptCallBatches, "on_compute");
}

void Lilliputian::VirtualMachine::executeOnLateMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_late");
}

void Lilliputian::VirtualMachine::executeOnFinalMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_final");
}

void Lilliputian::VirtualMachine::executeMethods(Vector<ScriptCallBatch> scriptCallBatches, const char* methodName)
{
	for (int i = 0; i < scriptCallBatches.size(); i++)
	{
		ScriptCallBatch scriptCallBatch = scriptCallBatches.at(i);

		for (int j = 0; j < scriptCallBatch.scripts.size(); j++)
		{
			String script = scriptCallBatch.scripts.at(j);
			ScriptingAPIs::bindEntity(
				scriptCallBatch.sceneTreeID,
				scriptCallBatch.entityID);

			try
			{
				if (this->modules.count(script))
				{
					this->modules.at(script).attr(methodName)();
				}
			}
			catch (const pybind11::error_already_set& e) //ignore method calls
			{
				std::cout << e.what() << std::endl;
			}

		}
	}
}