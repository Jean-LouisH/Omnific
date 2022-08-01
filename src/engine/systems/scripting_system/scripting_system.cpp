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
	this->scriptingLanguages.emplace("PythonVM", std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<PythonVM>(new PythonVM())));
	this->scriptingLanguages.emplace("CPPNative", std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<CPPNative>(new CPPNative())));
}

void Omnia::ScriptingSystem::initialize()
{
	for (auto scriptingLanguage : this->scriptingLanguages)
		scriptingLanguage.second->initialize();
}

void Omnia::ScriptingSystem::process(std::shared_ptr<Scene> scene)
{

}

void Omnia::ScriptingSystem::executeCommand(std::string command)
{
	std::shared_ptr<PythonVM> pythonVM = std::dynamic_pointer_cast<PythonVM>(this->scriptingLanguages.at("PythonVM"));
	if (pythonVM != nullptr)
		pythonVM->executeCommand(command);
}

void Omnia::ScriptingSystem::loadScriptModules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->loadScriptInstances();
	}
}

void Omnia::ScriptingSystem::executeOnStartMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnStartMethods();

		for (auto it : scene->getSceneTrees())
			it.second->clearStartEntityQueue();
	}
}

void Omnia::ScriptingSystem::executeOnInputMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnInputMethods();
	}
}

void Omnia::ScriptingSystem::executeOnLogicFrameMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnLogicFrameMethods();
	}
}

void Omnia::ScriptingSystem::executeOnComputeFrameMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnComputeFrameMethods();
	}
}

void Omnia::ScriptingSystem::executeOnOutputMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnOutputMethods();
	}
}

void Omnia::ScriptingSystem::executeOnFinishMethods(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->executeOnFinishMethods();

		for (auto it : scene->getSceneTrees())
			it.second->clearFinishEntityQueue();
	}
}

void Omnia::ScriptingSystem::setSceneSerializer(std::shared_ptr<SceneSerializer> sceneSerializer)
{
	ScriptContext::setSceneSerializer(sceneSerializer);
}

void Omnia::ScriptingSystem::setSceneStorage(std::shared_ptr<SceneStorage> sceneStorage)
{
	ScriptContext::setSceneStorage(sceneStorage);
}

void Omnia::ScriptingSystem::deinitialize()
{
	for (auto scriptingLanguage : this->scriptingLanguages)
		scriptingLanguage.second->deinitialize();
}