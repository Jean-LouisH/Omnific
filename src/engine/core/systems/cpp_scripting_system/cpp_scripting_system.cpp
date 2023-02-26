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

#include "cpp_scripting_system.hpp"
#include "core/singletons/entity_context.hpp"
#include <core/singletons/scene_storage.hpp>
#include <core/singletons/os/os.hpp>
#include <iostream>

typedef void(ScriptProcedure)(void);

void Omnia::CPPScriptingSystem::initialize()
{
	Logger& logger = OS::getLogger();
	logger.write("Initializing C++ Scripting System...");
}

void Omnia::CPPScriptingSystem::loadScriptModules(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
	{
		this->dynamicLibraryHandle = OS::getSharedLibraryAccess().open(this->nativeAssemblyFilename);
		this->execute("loadScriptInstances");
	}
}

void Omnia::CPPScriptingSystem::onStart(std::shared_ptr<Scene> scene)
{
	if (SceneStorage::hasActiveSceneChanged())
		this->loadScriptModules(scene);

	if (scene != nullptr)
		this->execute("onStart");
}

void Omnia::CPPScriptingSystem::onInput(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onInput");
}

void Omnia::CPPScriptingSystem::onEarly(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onEarly");
}

void Omnia::CPPScriptingSystem::onLogic(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onLogic");
}

void Omnia::CPPScriptingSystem::onCompute(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onCompute");
}

void Omnia::CPPScriptingSystem::onLate(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onLate");
}

void Omnia::CPPScriptingSystem::onFinish(std::shared_ptr<Scene> scene)
{
	if (scene != nullptr)
		this->execute("onFinish");
}

void Omnia::CPPScriptingSystem::execute(std::string methodName)
{
	if (this->dynamicLibraryHandle != nullptr)
	{
		ScriptProcedure* procedure =
			(ScriptProcedure*)OS::getSharedLibraryAccess().getProcedure(this->dynamicLibraryHandle, methodName.c_str());

		if (procedure != nullptr)
			procedure();
	}
}

void Omnia::CPPScriptingSystem::finalize()
{
	OS::getSharedLibraryAccess().close(this->dynamicLibraryHandle);
}