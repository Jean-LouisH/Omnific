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

void Omnific::SceneAPI::setSceneStorage(SceneStorage* sceneStorage)
{
	this->sceneStorage = sceneStorage;
}

void Omnific::SceneAPI::bindEntity(SceneID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Omnific::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Omnific::SceneAPI::hasComponent(std::string type)
{
	return this->getScene().getEntity(this->boundEntityID).componentIDs.count(type) > 0;
}

void Omnific::SceneAPI::preloadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Omnific::SceneAPI::loadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Omnific::SceneAPI::unloadScene(std::string sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Omnific::SceneAPI::changeToScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

Omnific::Entity& Omnific::SceneAPI::getEntity()
{
	return this->getScene().getEntity(this->boundEntityID);
}

Omnific::Scene& Omnific::SceneAPI::getScene()
{
	return sceneStorage->getActiveScene();
}

Omnific::Component& Omnific::SceneAPI::getComponent(std::string type)
{
	std::shared_ptr<Component> component = std::make_shared<Component>();
	
	Entity& entity = this->getScene().getEntity(this->boundEntityID);
	std::vector<std::shared_ptr<Component>>& components = this->getScene().getComponents();

	for (int i = 0; i < components.size(); i++)
		if (components.at(i)->getID() == entity.componentIDs.at(type))
			component = components.at(i);

	return *component;
}