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

#include "scripting_apis.hpp"
#include <os/os.hpp>

Esi::ScriptingAPIs* Esi::ScriptingAPIs::instance = nullptr;

void Esi::ScriptingAPIs::initialize()
{
	ScriptingAPIs* newInstance = getInstance();

	newInstance->commandLineAPI = new CommandLineAPI();
	newInstance->inputAPI = new InputAPI(&OS::getHid());
	newInstance->logAPI = new LogAPI(&OS::getLogger());
	newInstance->sceneAPI = new SceneAPI();
	newInstance->timeAPI = new TimeAPI(&OS::getProfiler());
	newInstance->windowAPI = new WindowAPI(&OS::getWindow());
}

void Esi::ScriptingAPIs::setSceneStorage(SceneStorage* sceneStorage)
{
	getInstance()->sceneAPI->setSceneStorage(sceneStorage);
}

void Esi::ScriptingAPIs::setConfiguration(Configuration* configuration)
{
	getInstance()->timeAPI->setTimeSettings(&configuration->timeSettings);
}

void Esi::ScriptingAPIs::bindEntity(SceneID sceneTreeID, EntityID entityID)
{
	getInstance()->sceneAPI->bindEntity(sceneTreeID, entityID);
}

Esi::CommandLineAPI& Esi::ScriptingAPIs::getCommandLineAPI()
{
	return *getInstance()->commandLineAPI;
}

Esi::InputAPI& Esi::ScriptingAPIs::getInputAPI()
{
	return *getInstance()->inputAPI;
}

Esi::LogAPI& Esi::ScriptingAPIs::getLogAPI()
{
	return *getInstance()->logAPI;
}

Esi::SceneAPI& Esi::ScriptingAPIs::getSceneAPI()
{
	return *getInstance()->sceneAPI;
}

Esi::TimeAPI& Esi::ScriptingAPIs::getTimeAPI()
{
	return *getInstance()->timeAPI;
}

Esi::WindowAPI& Esi::ScriptingAPIs::getWindowAPI()
{
	return *getInstance()->windowAPI;
}

std::string Esi::ScriptingAPIs::getDataDirectoryPath()
{
	return OS::getFileAccess().getDataDirectoryPath();
}

std::string Esi::ScriptingAPIs::getExecutableDirectoryPath()
{
	return OS::getFileAccess().getExecutableDirectoryPath();
}

Esi::ScriptingAPIs* Esi::ScriptingAPIs::getInstance()
{
	if (instance == nullptr)
		instance = new ScriptingAPIs();
	return instance;
}