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

Esi::Application::Application()
{
	this->scripting = std::shared_ptr<Scripting>(new Scripting());
	this->sceneStorage = std::shared_ptr<SceneStorage>(new SceneStorage());
	this->commandLine = std::shared_ptr<CommandLine>(new CommandLine(
		this->sceneSerializer.get(),
		this->sceneStorage.get()));

	ScriptingAPIs::initialize();
}

void Esi::Application::initialize()
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

		Image image = Image((dataDirectory + this->configuration->metadata.iconFilepath).c_str());
		OS::getWindow().changeIcon(image);
		image.unload();

		if (this->sceneSerializer->doesSceneExist(this->configuration->metadata.entrySceneFilepath))
		{
			entryScene = this->sceneSerializer->loadFromFile(this->configuration->metadata.entrySceneFilepath);
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
	std::cout << "\n\n\tEsi Debug Console Enabled";
	std::cout << "\n\nPress '`' in-application to write to command line via console.";
	std::cout << "\n\nTo see the list of commands, enter 'commands'.";
	std::cout << "\n\n";
#endif
}

void Esi::Application::executeOnStartMethods()
{
	if (this->sceneStorage->hasActiveSceneChanged())
		this->scripting->loadModules(this->sceneStorage->getActiveScene());

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnStartMethods(this->getActiveScene());
}

void Esi::Application::executeOnInputMethods()
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

	std::queue<PlayerID>& newlyLoadedPlayerIDs = OS::getHid().getNewlyLoadedPlayerIDs();

	if (!this->sceneStorage->isEmpty() && !newlyLoadedPlayerIDs.empty())
	{
		Scene& activeScene = this->getActiveScene();
		HapticSignalBuffer& hapticSignalBuffer = activeScene.getHapticSignalBuffer();

		while (!newlyLoadedPlayerIDs.empty())
		{
			/*
				For every newly detected controller, a pulse and silence signal is 
				fed to its haptics the amount of times that correspond to the player ID number.
			*/

			/*Warm up*/
			hapticSignalBuffer.publish(
				newlyLoadedPlayerIDs.front(),
				0.0,
				1000);

			for (int i = 0; i < newlyLoadedPlayerIDs.front() + 1; i++)
			{
				/*Silence*/
				hapticSignalBuffer.publish(
					newlyLoadedPlayerIDs.front(),
					0.0,
					250);

				/*Pulse*/
				hapticSignalBuffer.publish(
					newlyLoadedPlayerIDs.front(),
					1.0,
					250);
			}

			newlyLoadedPlayerIDs.pop();
		}
	}
	
}

void Esi::Application::executeOnFrameMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFrameMethods(this->getActiveScene());
}

void Esi::Application::executeOnComputeMethods()
{
	uint32_t msPerComputeUpdate = this->configuration->timeSettings.msPerComputeUpdate;

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnComputeMethods(this->getActiveScene());
}

void Esi::Application::executeOnOutputMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnOutputMethods(this->getActiveScene());
}

void Esi::Application::executeOnFinishMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFinishMethods(this->getActiveScene());

	this->getActiveScene().getEventBus().clear();
}

void Esi::Application::deinitialize()
{

}

Esi::Scene& Esi::Application::getActiveScene()
{
	return this->sceneStorage->getActiveScene();
}

Esi::Configuration& Esi::Application::getConfiguration()
{
	return *this->configuration;
}