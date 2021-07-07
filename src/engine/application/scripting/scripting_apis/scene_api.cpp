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

void Lilliputian::SceneAPI::setSceneStorage(SceneStorage* sceneStorage)
{
	this->sceneStorage = sceneStorage;
}

void Lilliputian::SceneAPI::bindEntity(SceneID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Lilliputian::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Lilliputian::SceneAPI::thisHasComponent(ComponentVariant::Type type)
{
	return this->getThisScene().getEntity(this->boundEntityID).components.count(type) > 0;
}

void Lilliputian::SceneAPI::preloadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Lilliputian::SceneAPI::loadScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Lilliputian::SceneAPI::unloadScene(std::string sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Lilliputian::SceneAPI::changeToScene(std::string sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

Lilliputian::Entity& Lilliputian::SceneAPI::getThisEntity()
{
	return this->getThisScene().getEntity(this->boundEntityID);
}

Lilliputian::Scene& Lilliputian::SceneAPI::getThisScene()
{
	return sceneStorage->getActiveScene();
}

Lilliputian::ComponentVariant& Lilliputian::SceneAPI::getThisComponentVariant(ComponentVariant::Type type)
{
	ComponentVariant* componentVariant = nullptr;
	
	Entity& entity = this->getThisScene().getEntity(this->boundEntityID);
	std::vector<ComponentVariant>& componentVariants = this->getThisScene().getComponentVariants();

	for (int i = 0; i < componentVariants.size(); i++)
		if (componentVariants.at(i).getID() == entity.components.at(type))
			componentVariant = &componentVariants.at(i);

	return *componentVariant;
}