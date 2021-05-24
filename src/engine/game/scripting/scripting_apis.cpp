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

Lilliputian::ScriptingAPIs* Lilliputian::ScriptingAPIs::instance = nullptr;

void Lilliputian::ScriptingAPIs::initialize()
{
	ScriptingAPIs* newInstance = getInstance();

	newInstance->commandLineAPI = new CommandLineAPI();
	newInstance->fileAPI = new FileAPI();
	newInstance->inputAPI = new InputAPI();
	newInstance->logAPI = new LogAPI();
	newInstance->renderAPI = new RenderAPI();
	newInstance->sceneAPI = new SceneAPI();
	newInstance->timeAPI = new TimeAPI();
	newInstance->windowAPI = new WindowAPI();

	newInstance->fileAPI->initialize(&OS::getFileAccess());
	newInstance->inputAPI->initialize(&OS::getHid());
	newInstance->windowAPI->initialize(&OS::getWindow());
}

void Lilliputian::ScriptingAPIs::bindScene(SceneForest* scene)
{
	getInstance()->sceneAPI->initialize(scene);
}

void Lilliputian::ScriptingAPIs::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	getInstance()->sceneAPI->bindEntity(sceneTreeID, entityID);
}

Lilliputian::CommandLineAPI& Lilliputian::ScriptingAPIs::getCommandLineAPI()
{
	return *getInstance()->commandLineAPI;
}

Lilliputian::FileAPI& Lilliputian::ScriptingAPIs::getFileAPI()
{
	return *getInstance()->fileAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPIs::getInputAPI()
{
	return *getInstance()->inputAPI;
}

Lilliputian::LogAPI& Lilliputian::ScriptingAPIs::getLogAPI()
{
	return *getInstance()->logAPI;
}

Lilliputian::RenderAPI& Lilliputian::ScriptingAPIs::getRenderAPI()
{
	return *getInstance()->renderAPI;
}

Lilliputian::SceneAPI& Lilliputian::ScriptingAPIs::getSceneAPI()
{
	return *getInstance()->sceneAPI;
}

Lilliputian::TimeAPI& Lilliputian::ScriptingAPIs::getTimeAPI()
{
	return *getInstance()->timeAPI;
}

Lilliputian::WindowAPI& Lilliputian::ScriptingAPIs::getWindowAPI()
{
	return *getInstance()->windowAPI;
}

Lilliputian::ScriptingAPIs* Lilliputian::ScriptingAPIs::getInstance()
{
	if (instance == nullptr)
		instance = new ScriptingAPIs();
	return instance;
}