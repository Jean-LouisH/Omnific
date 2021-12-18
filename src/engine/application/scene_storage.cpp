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

#include "scene_storage.hpp"

void Esi::SceneStorage::addScene(std::string sceneName, Scene scene)
{
	if (this->activeSceneName == "")
	{
		this->activeSceneName = sceneName;
		this->activeSceneChanged = true;
	}

	this->scenes.emplace(sceneName, scene);
}
void Esi::SceneStorage::removeScene(std::string sceneName)
{
	if (sceneName != this->activeSceneName)
	{
		if (this->scenes.count(sceneName))
		{
			this->scenes.at(sceneName).unload();
			this->scenes.erase(sceneName);
		}
	}
}
void Esi::SceneStorage::replaceActiveScene(std::string sceneName, Scene scene)
{
	if (sceneName != this->activeSceneName)
	{
		std::string oldSceneName = this->activeSceneName;
		this->addScene(sceneName, scene);
		this->changeToScene(sceneName);
		this->removeScene(oldSceneName);
		this->activeSceneChanged = true;
	}
}
void Esi::SceneStorage::changeToScene(std::string sceneName)
{
	if (this->scenes.count(sceneName))
		this->activeSceneName = sceneName;

	this->activeSceneChanged = true;
}

Esi::Scene& Esi::SceneStorage::getActiveScene()
{
	return this->scenes.at(this->activeSceneName);
}

std::string Esi::SceneStorage::getActiveSceneName()
{
	return this->activeSceneName;
}

bool Esi::SceneStorage::isEmpty()
{
	return this->scenes.empty();
}

bool Esi::SceneStorage::hasActiveSceneChanged()
{
	bool result = this->activeSceneChanged;
	this->activeSceneChanged = false;
	return result;
}