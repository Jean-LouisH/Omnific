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

#include "singletons/scene_storage.hpp"
#include "os/os.hpp"

Omnia::SceneStorage* Omnia::SceneStorage::instance = nullptr;

void Omnia::SceneStorage::addScene(std::string sceneName, std::shared_ptr<Scene> scene)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneStorage->activeSceneName == "")
	{
		sceneStorage->activeSceneName = sceneName;
		sceneStorage->activeSceneChanged = true;
		OS::getLogger().write("Added scene \"" + sceneName + "\"");
	}

	sceneStorage->scenes.emplace(sceneName, scene);
}
void Omnia::SceneStorage::removeScene(std::string sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneName != sceneStorage->activeSceneName)
	{
		if (sceneStorage->scenes.count(sceneName))
		{
			sceneStorage->scenes.erase(sceneName);
			OS::getLogger().write("Removed scene \"" + sceneName + "\"");
		}
	}
}
void Omnia::SceneStorage::replaceActiveScene(std::string sceneName, std::shared_ptr<Scene> scene)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneName != sceneStorage->activeSceneName)
	{
		std::string oldSceneName = sceneStorage->activeSceneName;
		sceneStorage->addScene(sceneName, scene);
		sceneStorage->changeToScene(sceneName);
		sceneStorage->removeScene(oldSceneName);
		sceneStorage->activeSceneChanged = true;
		OS::getLogger().write("Replaced active scene with \"" + sceneName + "\"");
	}
}
void Omnia::SceneStorage::changeToScene(std::string sceneName)
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();

	if (sceneStorage->scenes.count(sceneName))
	{
		sceneStorage->activeSceneName = sceneName;
		OS::getLogger().write("Changed to scene \"" + sceneName + "\"");
	}

	sceneStorage->activeSceneChanged = true;
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

bool Omnia::SceneStorage::isEmpty()
{
	return SceneStorage::getInstance()->scenes.empty();
}

bool Omnia::SceneStorage::hasActiveSceneChanged()
{
	SceneStorage* sceneStorage = SceneStorage::getInstance();
	bool result = sceneStorage->activeSceneChanged;
	sceneStorage->activeSceneChanged = false;
	return result;
}

Omnia::SceneStorage* Omnia::SceneStorage::getInstance()
{
	if (instance == nullptr)
		instance = new SceneStorage();
	return instance;
}