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

void Omnia::PythonVM::executeCommand(std::string command)
{
	pybind11::exec("from omnia import *");
	pybind11::exec(command);
}

void Omnia::PythonVM::loadScriptModules()
{
	this->pythonScriptInstances.clear();

	pybind11::module_ sys = pybind11::module_::import("sys");
	pybind11::object path = sys.attr("path");
	std::set<std::string> addedPaths;

	for (auto it : ScriptContext::getScene().getSceneTrees())
	{
		std::vector<std::shared_ptr<ScriptCollection>> scriptCollections = it.second->getComponentsByType<ScriptCollection>();
		size_t scriptCollectionsCount = scriptCollections.size();

		for (std::shared_ptr<ScriptCollection> scriptCollection : it.second->getComponentsByType<ScriptCollection>())
		{
			for (std::shared_ptr<Script> script : scriptCollection->scripts)
			{
				std::string scriptFilepath = script->getName();

				if (script->getType() == Omnia::PythonVMScript::TYPE_STRING)
				{
					try
					{
						if (addedPaths.count(scriptFilepath) == 0)
						{
							std::string newPath = OS::getFileAccess().getExecutableDirectoryPath() +
								"//data//" + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#ifdef _DEBUG
							newPath = OS::getFileAccess().getExecutableDirectoryPath();
							newPath = newPath.substr(0, newPath.find("out\\build\\x64-Debug\\src\\main"));
							std::string dataFolder = OS::getFileAccess().getDataDirectoryPath();
							dataFolder = dataFolder.substr(dataFolder.find("data/"), dataFolder.size() - 1);
							newPath += dataFolder + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#endif
							pybind11::str newPathObj = pybind11::str(newPath);
							newPathObj = newPathObj.attr("replace")("//", "/");
							newPathObj = newPathObj.attr("replace")("/", "\\");
							newPath = newPathObj.cast<std::string>();

							path.attr("insert")(0, newPath);
							addedPaths.emplace(newPath);
						}

						std::string moduleName = OS::getFileAccess().getFileNameWithoutExtension(scriptFilepath);
						pybind11::module_ newPybind11Module = pybind11::module_::import(moduleName.c_str());

						PythonScriptInstance scriptInstance;
						std::vector<std::string> methodNames = { "on_start", "on_input", "on_logic_frame", "on_compute_frame", "on_output", "on_finish" };
						scriptInstance.setData(newPybind11Module.attr("omnia_script")());

						for (int i = 0; i < methodNames.size(); i++)
						{
							try
							{
								std::string methodName = methodNames.at(i);
								pybind11::object test = scriptInstance.test(methodName.c_str());
								scriptInstance.setCallable(methodName);
							}
							catch (const pybind11::error_already_set& e) //using the exception catch to detect if method is not callable
							{

							}
						}

						this->pythonScriptInstances.emplace(scriptFilepath + std::to_string(scriptCollection->getEntityID()), scriptInstance);
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

void Omnia::PythonVM::executeOnStartMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeQueuedMethods(it.second->getStartEntityQueue(), it.second, "on_start");
}

void Omnia::PythonVM::executeOnInputMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeUpdateMethods(it.second, "on_input");
}

void Omnia::PythonVM::executeOnLogicFrameMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeUpdateMethods(it.second, "on_logic_frame");
}

void Omnia::PythonVM::executeOnComputeFrameMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeUpdateMethods(it.second, "on_compute_frame");
}

void Omnia::PythonVM::executeOnOutputMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeUpdateMethods(it.second, "on_output");
}

void Omnia::PythonVM::executeOnFinishMethods()
{
	for (auto it : ScriptContext::getScene().getSceneTrees())
		this->executeQueuedMethods(it.second->getFinishEntityQueue(), it.second, "on_finish");
}

void Omnia::PythonVM::deinitialize()
{
	pybind11::finalize_interpreter();
}

void Omnia::PythonVM::executeQueuedMethods(
	std::queue<EntityID> entityQueue, 
	std::shared_ptr<SceneTree> sceneTree, 
	const char* methodName)
{
	while (!entityQueue.empty())
	{
		std::shared_ptr<Entity> entity = sceneTree->getEntity(entityQueue.front());
		std::shared_ptr<ScriptCollection> scriptCollection = sceneTree->getComponent<ScriptCollection>(entity->getID());
		if (scriptCollection != nullptr)
		{
			this->bindAndCall(scriptCollection, sceneTree->getID(), scriptCollection->getEntityID(), methodName);
		}
		entityQueue.pop();
	}
}

void Omnia::PythonVM::executeUpdateMethods(std::shared_ptr<SceneTree> sceneTree, const char* methodName)
{
	std::vector<std::shared_ptr<ScriptCollection>> scriptCollections = sceneTree->getComponentsByType<ScriptCollection>();
	size_t scriptCollectionsCount = scriptCollections.size();

	for (size_t i = 0; i < scriptCollectionsCount; i++)
	{
		std::shared_ptr<ScriptCollection> scriptCollection = scriptCollections.at(i);
		this->bindAndCall(scriptCollection, sceneTree->getID(), scriptCollection->getEntityID(), methodName);
	}
}

void Omnia::PythonVM::bindAndCall(
	std::shared_ptr<ScriptCollection> scriptCollection,
	SceneTreeID sceneTreeID,
	EntityID entityID,
	const char* methodName)
{
	for (size_t j = 0; j < scriptCollection->scripts.size(); j++)
	{
		std::string scriptPath = scriptCollection->scripts.at(j)->getName() + std::to_string(entityID);

		if (this->pythonScriptInstances.count(scriptPath))
		{
			if (this->pythonScriptInstances.at(scriptPath).hasCallable(methodName))
			{
				ScriptContext::bindEntity(
					sceneTreeID,
					entityID);

				try
				{
					this->pythonScriptInstances.at(scriptPath).call(methodName);
				}
				catch (const pybind11::error_already_set& e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
	}
}