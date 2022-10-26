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

#include "core/singletons/scene_storage.hpp"
#include "os/os.hpp"

Omnia::SceneStorage* Omnia::SceneStorage::instance = nullptr;

void Omnia::SceneStorage::addScene(std::shared_ptr<Scene> scene)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	std::string sceneName = scene->getName();

	if (!sceneStorage->hasScene(sceneName))
	{
		sceneStorage->scenes.emplace(sceneName, scene);
		OS::getLogger().write("Added Scene: \"" + sceneName + "\"");
	}
	else
	{
		OS::getLogger().write("Error: Attempted to add Scene: \"" + sceneName + "\" which already exists.");
	}
}

void Omnia::SceneStorage::addScene(std::string filepath)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (!sceneStorage->hasScene(filepath))
	{
		std::shared_ptr<Scene> scene(new Scene(filepath));
		sceneStorage->addScene(scene);
	}
}

void Omnia::SceneStorage::addAndChangeToScene(std::shared_ptr<Scene> scene)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	sceneStorage->addScene(scene);
	sceneStorage->changeToScene(scene->getName());
}

void Omnia::SceneStorage::addAndChangeToScene(std::string filepath)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	sceneStorage->addScene(filepath);
	sceneStorage->changeToScene(filepath);
}

void Omnia::SceneStorage::removeScene(std::string sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneName != "")
	{
		if (sceneStorage->hasScene(sceneName))
		{
			/*Removed Scenes are stored to allow the ouput thread to access their data
			before registering a new active scene, without the use of synchronization. */
			std::shared_ptr<Scene> sceneToRemove = sceneStorage->scenes.at(sceneName);

			if (sceneStorage->removedScenes.count(sceneName))
			{
				sceneStorage->removedScenes.erase(sceneName);
			}

			sceneStorage->removedScenes.emplace(sceneName, sceneToRemove);

			if (sceneName == sceneStorage->getActiveSceneName())
			{
				sceneStorage->changeToScene("");
			}

			sceneStorage->scenes.erase(sceneName);
			OS::getLogger().write("Removed Scene: \"" + sceneName + "\"");
		}
	}
	else
	{
		OS::getLogger().write("Error: Attempted to remove empty Scene.");
	}
}

void Omnia::SceneStorage::changeToScene(std::string sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneStorage->hasScene(sceneName))
	{
		sceneStorage->activeSceneName = sceneName;
		OS::getLogger().write("Changed to Scene: \"" + sceneName + "\"");
	}

	sceneStorage->activeSceneChanged = true;
}

void Omnia::SceneStorage::reloadActiveScene()
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	std::string activeSceneName = sceneStorage->getActiveSceneName();

	if (activeSceneName != "" &&
		sceneStorage->hasScene(activeSceneName))
	{
		if (sceneStorage->removedScenes.count(activeSceneName))
		{
			std::shared_ptr<Scene> previouslyRemovedScene = sceneStorage->removedScenes.at(activeSceneName);
			sceneStorage->removeScene(activeSceneName);
			previouslyRemovedScene->reload();
			sceneStorage->addAndChangeToScene(previouslyRemovedScene);
		}
		else
		{
			sceneStorage->removeScene(activeSceneName);
			sceneStorage->addAndChangeToScene(std::shared_ptr<Scene>(new Scene(activeSceneName)));
		}
	}
}

std::shared_ptr<Omnia::Scene> Omnia::SceneStorage::getActiveScene()
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	return sceneStorage->scenes.at(sceneStorage->activeSceneName);
}

std::string Omnia::SceneStorage::getActiveSceneName()
{
	return SceneStorage::getInstance()->activeSceneName;
}

bool Omnia::SceneStorage::hasNoScenes()
{
	/*Accounting for Scenes other than the dummy scene.*/
	return SceneStorage::getInstance()->scenes.size() < 2;
}

bool Omnia::SceneStorage::hasScene(std::string sceneName)
{
	return SceneStorage::getInstance()->scenes.count(sceneName);
}

bool Omnia::SceneStorage::hasActiveSceneChanged()
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	bool result = sceneStorage->activeSceneChanged;
	sceneStorage->activeSceneChanged = false;
	return result;
}

void Omnia::SceneStorage::clearScenes()
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	sceneStorage->scenes.clear();
	sceneStorage->activeSceneName = "";
}

std::shared_ptr<Omnia::Scene> Omnia::SceneStorage::getSceneByName(std::string sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	std::shared_ptr<Scene> scene;

	if (sceneStorage->hasScene(sceneName))
	{
		scene = sceneStorage->scenes.at(sceneName);
	}

	return scene;
}

Omnia::SceneStorage* Omnia::SceneStorage::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneStorage();
		instance->addScene(std::shared_ptr<Scene>(new Scene()));
	}
	return instance;
}