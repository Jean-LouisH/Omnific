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

Omnific::ScriptingAPIs* Omnific::ScriptingAPIs::instance = nullptr;

void Omnific::ScriptingAPIs::initialize()
{
	ScriptingAPIs* newInstance = getInstance();

	newInstance->commandLineAPI = new CommandLineAPI();
	newInstance->inputAPI = new InputAPI(&OS::getHid());
	newInstance->logAPI = new LogAPI(&OS::getLogger());
	newInstance->sceneAPI = new SceneAPI();
	newInstance->timeAPI = new TimeAPI(&OS::getProfiler());
	newInstance->windowAPI = new WindowAPI(&OS::getWindow());
}

void Omnific::ScriptingAPIs::setSceneStorage(SceneStorage* sceneStorage)
{
	getInstance()->sceneAPI->setSceneStorage(sceneStorage);
}

void Omnific::ScriptingAPIs::setConfiguration(Configuration* configuration)
{
	getInstance()->timeAPI->setTimeSettings(&configuration->timeSettings);
}

void Omnific::ScriptingAPIs::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	getInstance()->sceneAPI->bindEntity(sceneTreeID, entityID);
}

Omnific::CommandLineAPI& Omnific::ScriptingAPIs::getCommandLineAPI()
{
	return *getInstance()->commandLineAPI;
}

Omnific::InputAPI& Omnific::ScriptingAPIs::getInputAPI()
{
	return *getInstance()->inputAPI;
}

Omnific::LogAPI& Omnific::ScriptingAPIs::getLogAPI()
{
	return *getInstance()->logAPI;
}

Omnific::SceneAPI& Omnific::ScriptingAPIs::getSceneAPI()
{
	return *getInstance()->sceneAPI;
}

Omnific::TimeAPI& Omnific::ScriptingAPIs::getTimeAPI()
{
	return *getInstance()->timeAPI;
}

Omnific::WindowAPI& Omnific::ScriptingAPIs::getWindowAPI()
{
	return *getInstance()->windowAPI;
}

std::string Omnific::ScriptingAPIs::getDataDirectoryPath()
{
	return OS::getFileAccess().getDataDirectoryPath();
}

std::string Omnific::ScriptingAPIs::getExecutableDirectoryPath()
{
	return OS::getFileAccess().getExecutableDirectoryPath();
}

Omnific::ScriptingAPIs* Omnific::ScriptingAPIs::getInstance()
{
	if (instance == nullptr)
		instance = new ScriptingAPIs();
	return instance;
}