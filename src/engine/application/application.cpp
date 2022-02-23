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

#include "application.hpp"
#include "boot_loader.hpp"
#include "utilities/constants.hpp"
#include <iostream>

Omnific::Application::Application()
{
	this->scripting = std::shared_ptr<Scripting>(new Scripting());
	this->sceneStorage = std::shared_ptr<SceneStorage>(new SceneStorage());
	this->commandLine = std::shared_ptr<CommandLine>(new CommandLine(this->sceneSerializer, this->sceneStorage));

	ScriptingAPIs::initialize();
}

void Omnific::Application::initialize()
{
	BootLoader bootLoader;
	Scene entryScene;
	std::string dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	std::string bootFilename = "boot.yml";
	std::string bootFilepath = dataDirectory + bootFilename;

	if (OS::getFileAccess().exists(bootFilepath))
	{
		this->configuration = std::shared_ptr<Configuration>(bootLoader.loadFromFile(bootFilepath));
#ifdef _DEBUG
		std::string debugDataFilepath = DEBUG_DATA_FILEPATH;
		std::string debugEditorDataFilepath = DEBUG_EDITOR_DATA_FILEPATH;
		if (debugDataFilepath == debugEditorDataFilepath)
			this->configuration->metadata.entrySceneFilepath = "assets/scenes/debug.yml";
#endif
		OS::getFileAccess().setDataDirectory(dataDirectory);
		this->sceneSerializer = std::shared_ptr<SceneSerializer>(new SceneSerializer(dataDirectory));
		ScriptingAPIs::getSceneAPI().setSceneSerializer(this->sceneSerializer.get());

		Image image = Image(dataDirectory + this->configuration->metadata.iconFilepath);
		OS::getWindow().changeIcon(image);

		if (this->sceneSerializer->doesSceneExist(this->configuration->metadata.entrySceneFilepath))
		{
			entryScene = this->sceneSerializer->deserialize(this->configuration->metadata.entrySceneFilepath);
			this->sceneStorage->addScene(this->configuration->metadata.entrySceneFilepath, entryScene);
			this->scripting->setSceneStorage(this->sceneStorage.get());
		}
	}
	else
	{
		this->configuration = std::shared_ptr<Configuration>(new Configuration());
		this->configuration->isLoaded = false;
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\n\tOmnific Debug Console Enabled";
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\nTo see the list of commands, enter 'commands'.";
	std::cout << "\n\n";
#endif
}

void Omnific::Application::executeOnStartMethods()
{
	if (this->sceneStorage->hasActiveSceneChanged())
		this->scripting->onModifiedScriptInstance(this->sceneStorage->getActiveScene());

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnStartMethods(this->getActiveScene());
}

void Omnific::Application::executeOnInputMethods()
{
#ifdef DEBUG_CONSOLE_ENABLED
	if (OS::getHid().hasRequestedCommandLine())
	{
		std::string command;

		OS::getWindow().hide();
		std::cout << std::endl << ">";
		std::getline(std::cin, command);
		this->commandLine->execute(command);
		OS::getWindow().show();
	}
#endif

	if (!this->sceneStorage->isEmpty() && OS::getHid().getHasDetectedInputChanges())
		this->scripting->executeOnInputMethods(this->getActiveScene());	
}

void Omnific::Application::executeOnFrameMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFrameMethods(this->getActiveScene());
}

void Omnific::Application::executeOnComputeMethods()
{
	uint32_t msPerComputeUpdate = this->configuration->timeSettings.msPerComputeUpdate;

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnComputeMethods(this->getActiveScene());
}

void Omnific::Application::executeOnOutputMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnOutputMethods(this->getActiveScene());
}

void Omnific::Application::executeOnFinishMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFinishMethods(this->getActiveScene());

	this->getActiveScene().getEventBus().clear();
}

void Omnific::Application::deinitialize()
{

}

Omnific::Scene& Omnific::Application::getActiveScene()
{
	return this->sceneStorage->getActiveScene();
}

Omnific::Configuration& Omnific::Application::getConfiguration()
{
	return *this->configuration;
}