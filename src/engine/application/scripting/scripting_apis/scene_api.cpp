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

#include "scene_api.hpp"

void Esi::SceneAPI::setSceneStorage(SceneStorage* sceneStorage)
{
	this->sceneStorage = sceneStorage;
}

void Esi::SceneAPI::bindEntity(SceneID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Esi::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Esi::SceneAPI::thisHasComponent(std::string type)
{
	return this->getThisScene().getEntity(this->boundEntityID).components.count(type) > 0;
}

void Esi::SceneAPI::preloadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Esi::SceneAPI::loadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Esi::SceneAPI::unloadScene(std::string sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Esi::SceneAPI::changeToScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

Esi::Entity& Esi::SceneAPI::getThisEntity()
{
	return this->getThisScene().getEntity(this->boundEntityID);
}

Esi::Scene& Esi::SceneAPI::getThisScene()
{
	return sceneStorage->getActiveScene();
}

Esi::Component& Esi::SceneAPI::getThisComponent(std::string type)
{
	std::shared_ptr<Component> component = std::make_shared<Component>();
	
	Entity& entity = this->getThisScene().getEntity(this->boundEntityID);
	std::vector<std::shared_ptr<Component>>& components = this->getThisScene().getComponents();

	for (int i = 0; i < components.size(); i++)
		if (components.at(i)->getID() == entity.components.at(type))
			component = components.at(i);

	return *component;
}