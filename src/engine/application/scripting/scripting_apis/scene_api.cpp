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

void Omnia::SceneAPI::setSceneStorage(SceneStorage* sceneStorage)
{
	this->sceneStorage = sceneStorage;
}

void Omnia::SceneAPI::bindEntity(SceneID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Omnia::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Omnia::SceneAPI::hasComponent(std::string type)
{
	return this->getSceneTree().getEntity(this->boundEntityID).componentIDs.count(type) > 0;
}

void Omnia::SceneAPI::preloadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Omnia::SceneAPI::loadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->deserialize(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Omnia::SceneAPI::unloadScene(std::string sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Omnia::SceneAPI::changeToScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

Omnia::Entity& Omnia::SceneAPI::getEntity()
{
	return this->getSceneTree().getEntity(this->boundEntityID);
}

Omnia::Scene& Omnia::SceneAPI::getScene()
{
	return sceneStorage->getActiveScene();
}

Omnia::SceneTree& Omnia::SceneAPI::getSceneTree()
{
	return this->getScene().getSceneTrees().at(this->boundSceneTreeID);
}

Omnia::Component& Omnia::SceneAPI::getComponent(std::string type)
{
	std::shared_ptr<Component> component = std::make_shared<Component>();
	
	Entity& entity = this->getSceneTree().getEntity(this->boundEntityID);
	std::vector<std::shared_ptr<Component>>& components = this->getSceneTree().getComponents();

	for (int i = 0; i < components.size(); i++)
		if (components.at(i)->getID() == entity.componentIDs.at(type))
			component = components.at(i);

	return *component;
}