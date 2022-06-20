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

#include "python_vm.hpp"
#include "embedded_module.hpp"
#include <systems/scripting_system/script_context.hpp>
#include <iostream>
#include <set>

Omnia::PythonVM::PythonVM()
{

}

Omnia::PythonVM::~PythonVM()
{

}

void Omnia::PythonVM::initialize()
{
	pybind11::initialize_interpreter();
}

void Omnia::PythonVM::loadScriptModules(std::shared_ptr<Scene> scene)
{
	this->modules.clear();

	pybind11::module_ sys = pybind11::module_::import("sys");
	pybind11::object path = sys.attr("path");
	std::set<std::string> addedPaths;
	std::set<std::string> scripts;

	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		std::vector<std::shared_ptr<ScriptCollection>> scriptCollections = it->second->getComponentsByType<ScriptCollection>();
		size_t scriptCollectionsCount = scriptCollections.size();

		for (size_t i = 0; i < scriptCollectionsCount; i++)
		{
			std::shared_ptr<ScriptCollection> scriptCollection = scriptCollections.at(i);
			std::vector<std::string> entityScripts;

			for (size_t j = 0; j < scriptCollection->scripts.size(); j++)
				entityScripts.push_back(scriptCollection->scripts.at(j)->getName());

			for (int j = 0; j < entityScripts.size(); j++)
				scripts.emplace(entityScripts.at(j));
		}

		for (auto it = scripts.begin(); it != scripts.end(); it++)
		{
			try
			{
				std::string scriptFilepath = *it;

				if (addedPaths.count(scriptFilepath) == 0)
				{
					std::string newPath = OS::getFileAccess().getExecutableDirectoryPath() +
						"//data//" + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#ifdef _DEBUG
					newPath = OS::getFileAccess().getExecutableDirectoryPath();
					newPath = newPath.substr(0, newPath.find("out\\build\\x64-Debug\\src\\main"));
					std::string dataFolder = "data//editor//";
#if (DEBUG_DEMO_MODE)
					dataFolder = "data//demos//";
#endif
					newPath += dataFolder + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#endif

					pybind11::str newPathObj = pybind11::str(newPath);
					newPathObj = newPathObj.attr("replace")("//", "/");
					newPathObj = newPathObj.attr("replace")("/", "\\");
					newPath = newPathObj.cast<std::string>();

					path.attr("insert")(0, newPath);
					addedPaths.emplace(newPath);
				}

				Module newModule;
				std::vector<std::string> methodNames = { "on_start", "on_input", "on_logic_frame", "on_compute_frame", "on_output", "on_finish" };
				std::string moduleName = OS::getFileAccess().getFileNameWithoutExtension(scriptFilepath);
				pybind11::module_ newPybind11Module = pybind11::module_::import(moduleName.c_str());

				newModule.setData(newPybind11Module);

				for (int i = 0; i < methodNames.size(); i++)
				{
					try
					{
						std::string methodName = methodNames.at(i);
						pybind11::object test = newPybind11Module.attr(methodName.c_str());
						newModule.setCallable(methodName);
					}
					catch (const pybind11::error_already_set& e) //using the exception catch to detect if method is not callable
					{

					}
				}

				this->modules.emplace(moduleName, newModule);
			}
			catch (const pybind11::error_already_set& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}

	
}

void Omnia::PythonVM::executeOnStartMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_start");
}

void Omnia::PythonVM::executeOnInputMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_input");
}

void Omnia::PythonVM::executeOnLogicFrameMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_logic_frame");
}

void Omnia::PythonVM::executeOnComputeFrameMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_compute_frame");
}

void Omnia::PythonVM::executeOnOutputMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_output");
}

void Omnia::PythonVM::executeOnFinishMethods(std::vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_finish");
}

void Omnia::PythonVM::deinitialize()
{
	pybind11::finalize_interpreter();
}

void Omnia::PythonVM::executeMethods(std::vector<ScriptCallBatch> scriptCallBatches, const char* methodName)
{
	for (int i = 0; i < scriptCallBatches.size(); i++)
	{
		ScriptCallBatch scriptCallBatch = scriptCallBatches.at(i);

		for (int j = 0; j < scriptCallBatch.scripts.size(); j++)
		{
			std::string scriptPath = scriptCallBatch.scripts.at(j);
			std::string scriptName = OS::getFileAccess().getFileNameWithoutExtension(scriptPath);

			if (this->modules.count(scriptName))
			{
				if (this->modules.at(scriptName).hasCallable(methodName))
				{
					ScriptContext::bindEntity(
						scriptCallBatch.sceneTreeID,
						scriptCallBatch.entityID);

					try
					{
						this->modules.at(scriptName).call(methodName);
					}
					catch (const pybind11::error_already_set& e)
					{
						std::cout << e.what() << std::endl;
					}
				}
			}
		}
	}
}