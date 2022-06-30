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

#include "scripting_system.hpp"
#include "script_context.hpp"

Omnia::ScriptingSystem::ScriptingSystem()
{
	this->pythonVM = std::unique_ptr<PythonVM>(new PythonVM());
	this->cppNative = std::unique_ptr<CPPNative>(new CPPNative());
}

void Omnia::ScriptingSystem::initialize()
{
	this->pythonVM->initialize();
}

void Omnia::ScriptingSystem::process(std::shared_ptr<Scene> scene)
{

}

void Omnia::ScriptingSystem::loadScriptModules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		this->pythonVM->loadScriptModules(scene);
		this->cppNative->loadScriptModules();
	}
}

void Omnia::ScriptingSystem::executeOnStartMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		{
			this->pythonVM->executeOnStartMethods(it->second);
			it->second->clearStartEntityQueue();
		}
	}
}

void Omnia::ScriptingSystem::executeOnInputMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
			this->pythonVM->executeOnInputMethods(it->second);
	}
}

void Omnia::ScriptingSystem::executeOnLogicFrameMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
			this->pythonVM->executeOnLogicFrameMethods(it->second);
		this->cppNative->executeOnLogicFrameMethods();
	}
}

void Omnia::ScriptingSystem::executeOnComputeFrameMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
			this->pythonVM->executeOnComputeFrameMethods(it->second);
	}
}

void Omnia::ScriptingSystem::executeOnOutputMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
			this->pythonVM->executeOnOutputMethods(it->second);
	}
}

void Omnia::ScriptingSystem::executeOnFinishMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

		for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		{
			this->pythonVM->executeOnFinishMethods(it->second);
			it->second->clearFinishEntityQueue();
		}
	}
}

void Omnia::ScriptingSystem::setSceneStorage(std::shared_ptr<SceneStorage> sceneStorage)
{
	ScriptContext::setSceneStorage(sceneStorage);
}

void Omnia::ScriptingSystem::deinitialize()
{
	this->pythonVM->deinitialize();
}