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
#include "singletons/script_context.hpp"


void Omnia::ScriptingSystem::initialize()
{
	for (auto scriptingLanguage : this->scriptingLanguages)
		scriptingLanguage.second->initialize();
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

void Omnia::ScriptingSystem::onStart(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onStart();

		for (auto it : scene->getSceneTrees())
			it.second->clearStartEntityQueue();
	}
}

void Omnia::ScriptingSystem::onInput(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onInput();
	}
}

void Omnia::ScriptingSystem::onEarly(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onEarly();
	}
}

void Omnia::ScriptingSystem::onLogic(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onLogic();
	}
}

void Omnia::ScriptingSystem::onCompute(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onCompute();
	}
}

void Omnia::ScriptingSystem::onLate(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onLate();
	}
}

void Omnia::ScriptingSystem::onFinish(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		for (auto scriptingLanguage : this->scriptingLanguages)
			scriptingLanguage.second->onFinish();

		for (auto it : scene->getSceneTrees())
			it.second->clearFinishEntityQueue();
	}
}

void Omnia::ScriptingSystem::finalize()
{
	for (auto scriptingLanguage : this->scriptingLanguages)
		scriptingLanguage.second->finalize();
}