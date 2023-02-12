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
#include "core/singletons/entity_context.hpp"
#include <core/singletons/scene_storage.hpp>
#include <core/singletons/os/os.hpp>
#include <iostream>


void Omnia::ScriptingSystem::initialize()
{
	Logger& logger = OS::getLogger();
	logger.write("Initializing Scripting System...");

	for (auto scriptingLanguage : this->scriptingLanguages)
	{
		scriptingLanguage.second->initialize();
		logger.write("Loaded Scripting Language: " + scriptingLanguage.first);
	}
}

void Omnia::ScriptingSystem::executeCommand(std::string command)
{
#if ENABLE_NON_NATIVE_SCRIPTING_LANGUAGE
	std::shared_ptr<Python> python = std::dynamic_pointer_cast<Python>(this->scriptingLanguages.at(Python::TYPE_STRING));
	if (python != nullptr)
		python->executeCommand(command);
#endif
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
	if (SceneStorage::hasActiveSceneChanged())
		this->loadScriptModules(scene);

#ifdef DEBUG_CONSOLE_ENABLED
	if (OS::getInput().hasRequestedCommandLine())
	{
		std::string command;

		OS::getWindow().hide();
		std::cout << std::endl << ">";
		std::cin.ignore(1, '\n');
		std::getline(std::cin, command);
		this->executeCommand(command);
		OS::getWindow().show();
	}
#endif

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

	this->scriptingLanguages.clear();
}