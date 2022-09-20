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
#include <singletons/entity_context.hpp>
#include <iostream>
#include <set>

Omnia::PythonVM::PythonVM()
{
	this->type = TYPE_STRING;
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

void Omnia::PythonVM::loadScriptInstances()
{
	this->pythonScriptInstances.clear();

	pybind11::module_ sys = pybind11::module_::import("sys");
	pybind11::object path = sys.attr("path");
	std::set<std::string> addedPaths;

	for (auto it : EntityContext::getScene()->getSceneTrees())
	{
		for (std::shared_ptr<ScriptCollection> scriptCollection : it.second->getComponentsByType<ScriptCollection>())
		{
			for (std::shared_ptr<Script> script : scriptCollection->scripts)
			{
				std::string scriptFilepath = script->getName();

				if (script->getType() == Omnia::PythonScript::TYPE_STRING)
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
						std::vector<std::string> methodNames = { "on_start", "on_input", "on_early", "on_logic", "on_compute", "on_late", "on_finish" };
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

void Omnia::PythonVM::onStart()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeQueuedMethods(it.second->getStartEntityQueue(), it.second, "on_start");
}

void Omnia::PythonVM::onInput()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeUpdateMethods(it.second, "on_input");
}

void Omnia::PythonVM::onEarly()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeUpdateMethods(it.second, "on_early");
}

void Omnia::PythonVM::onLogic()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeUpdateMethods(it.second, "on_logic");
}

void Omnia::PythonVM::onCompute()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeUpdateMethods(it.second, "on_compute");
}

void Omnia::PythonVM::onLate()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeUpdateMethods(it.second, "on_late");
}

void Omnia::PythonVM::onFinish()
{
	for (auto it : EntityContext::getScene()->getSceneTrees())
		this->executeQueuedMethods(it.second->getFinishEntityQueue(), it.second, "on_finish");
}

void Omnia::PythonVM::finalize()
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
		std::shared_ptr<ScriptCollection> scriptCollection = sceneTree->getComponentByType<ScriptCollection>(entity->getID());
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
				EntityContext::bindEntity(
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