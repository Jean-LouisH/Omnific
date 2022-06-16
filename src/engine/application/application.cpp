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
#include <application/scripting/script_context.hpp>
#include <iostream>

Omnia::Application::Application()
{
	this->scripting = std::shared_ptr<Scripting>(new Scripting());
	this->sceneStorage = std::shared_ptr<SceneStorage>(new SceneStorage());
	this->commandLine = std::shared_ptr<CommandLine>(new CommandLine(this->sceneSerializer, this->sceneStorage));
}

void Omnia::Application::preloadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		std::shared_ptr<Scene> newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Omnia::Application::loadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		std::shared_ptr<Scene> newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Omnia::Application::unloadScene(std::string sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Omnia::Application::changeToScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

void Omnia::Application::initialize()
{
	BootLoader bootLoader;
	std::shared_ptr<Scene> entryScene;
	std::string dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	std::string bootFilename = "boot.yml";
	std::string bootFilepath = dataDirectory + bootFilename;

	if (OS::getFileAccess().exists(bootFilepath))
	{
		this->configuration = bootLoader.loadFromFile(bootFilepath);
#ifdef _DEBUG
		std::string debugDataFilepath = DEBUG_DATA_FILEPATH;
		std::string debugEditorDataFilepath = DEBUG_EDITOR_DATA_FILEPATH;
		if (debugDataFilepath == debugEditorDataFilepath)
			this->configuration->metadata.entrySceneFilepath = "assets/scenes/debug.yml";
#endif
		OS::getFileAccess().setDataDirectory(dataDirectory);
		this->sceneSerializer = std::shared_ptr<SceneSerializer>(new SceneSerializer(dataDirectory));

		Image image = Image(dataDirectory + this->configuration->metadata.iconFilepath);
		OS::getWindow().changeIcon(image);

		if (this->sceneSerializer->doesSceneExist(this->configuration->metadata.entrySceneFilepath))
		{
			entryScene = this->sceneSerializer->deserialize(this->configuration->metadata.entrySceneFilepath);
			this->sceneStorage->addScene(this->configuration->metadata.entrySceneFilepath, entryScene);
			this->scripting->setSceneStorage(this->sceneStorage);
		}
	}
	else
	{
		this->configuration = std::shared_ptr<Configuration>(new Configuration());
		this->configuration->isLoaded = false;
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\n\n\tOmnia Debug Console Enabled";
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\nTo see the list of commands, enter 'commands'.";
	std::cout << "\n\n";
#endif
}

void Omnia::Application::executeOnStartMethods()
{
	if (this->sceneStorage->hasActiveSceneChanged())
		this->scripting->onModifiedScriptInstance(this->sceneStorage->getActiveScene());

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnStartMethods(this->getActiveScene());
}

void Omnia::Application::executeOnInputMethods()
{
#ifdef DEBUG_CONSOLE_ENABLED
	if (OS::getInput().hasRequestedCommandLine())
	{
		std::string command;

		OS::getWindow().hide();
		std::cout << std::endl << ">";
		std::getline(std::cin, command);
		this->commandLine->execute(command);
		OS::getWindow().show();
	}
#endif

	if (!this->sceneStorage->isEmpty() && OS::getInput().getHasDetectedInputChanges())
		this->scripting->executeOnInputMethods(this->getActiveScene());	
}

void Omnia::Application::executeOnLogicFrameMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnLogicFrameMethods(this->getActiveScene());
}

void Omnia::Application::executeOnComputeFrameMethods()
{
	uint32_t msPerComputeUpdate = this->configuration->timeSettings.msPerComputeUpdate;

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnComputeFrameMethods(this->getActiveScene());
}

void Omnia::Application::executeOnOutputMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnOutputMethods(this->getActiveScene());
}

void Omnia::Application::executeOnFinishMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFinishMethods(this->getActiveScene());

	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = this->getActiveScene()->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		it->second->getEventBus()->clear();
}

void Omnia::Application::deinitialize()
{

}

std::shared_ptr<Omnia::Scene> Omnia::Application::getActiveScene()
{
	return this->sceneStorage->getActiveScene();
}

std::shared_ptr<Omnia::Configuration> Omnia::Application::getConfiguration()
{
	return this->configuration;
}