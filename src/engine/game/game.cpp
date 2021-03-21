//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "game.hpp"
#include "boot_loader.hpp"
#include "utilities/constants.hpp"

Lilliputian::Game::Game(
	OS* os,
	Profiler* profiler)
{
	this->os = os;
	this->profiler = profiler;
	this->scriptingAPIs = new ScriptingAPIs();
	this->scriptRegistry = new ScriptRegistry(
		this->scriptingAPIs,
		&this->scripts);
}

void Lilliputian::Game::initialize()
{
	BootLoader bootLoader;
	SceneForest entryScene;
	String dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	String bootFilename = "boot.yml";
	String bootFilepath = dataDirectory + bootFilename;

	if (this->os->fileAccess().exists(bootFilepath))
	{
		this->_configuration = bootLoader.loadFromFile(bootFilepath);
		this->sceneSerializer = new SceneSerializer(dataDirectory);
		entryScene = this->sceneSerializer->loadFromFile(_configuration->entrySceneFilepath);
		this->activeSceneStack.emplace(entryScene);
	}
	else
	{
		this->_configuration = new BootConfiguration();
		this->_configuration->isLoaded = false;
	}
}

void Lilliputian::Game::executeOnStartMethods()
{

}

void Lilliputian::Game::executeOnInputMethods()
{

}

void Lilliputian::Game::executeOnFrameMethods()
{
	if (this->activeSceneStack.size() > 0)
		this->activeSceneStack.top().executeFrameLogic();
}

void Lilliputian::Game::executeOnComputeMethods()
{
	uint32_t msPerComputeUpdate = this->_configuration->msPerComputeUpdate;
}

void Lilliputian::Game::executeOnLateMethods()
{

}

void Lilliputian::Game::executeOnFinalMethods()
{

}

void Lilliputian::Game::deinitialize()
{
	delete this->_configuration;
	delete this->scriptingAPIs;
	delete this->scriptRegistry;
	delete this->sceneSerializer;
}

Lilliputian::ScriptRegistry& Lilliputian::Game::getScriptRegistry()
{
	return *this->scriptRegistry;
}

Lilliputian::SceneForest& Lilliputian::Game::getActiveScene()
{
	return this->activeSceneStack.top();
}

Lilliputian::BootConfiguration& Lilliputian::Game::configuration()
{
	return *this->_configuration;
}