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

void Lilliputian::SceneAPI::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Lilliputian::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Lilliputian::SceneAPI::hasComponent(ComponentVariant::Type type)
{
	bool result = false;
	std::vector<SceneTree2D>& sceneTree2Ds = this->sceneStorage->getActiveScene().getSceneTree2Ds();

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			if (sceneTree2Ds.at(i).getEntity2D(this->boundEntityID).components.count(type) > 0)
				return true;

	return result;
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

Lilliputian::Entity2D& Lilliputian::SceneAPI::getThisEntity2D()
{
	return this->getThisSceneTree2D().getEntity2D(this->boundEntityID);
}

Lilliputian::SceneTree2D& Lilliputian::SceneAPI::getThisSceneTree2D()
{
	return this->sceneStorage->getActiveScene().getSceneTree2Ds().at(this->boundSceneTreeID);
}

Lilliputian::Scene& Lilliputian::SceneAPI::getThisScene()
{
	return sceneStorage->getActiveScene();
}

Lilliputian::ComponentVariant& Lilliputian::SceneAPI::getThisComponentVariant(ComponentVariant::Type type)
{
	ComponentVariant* componentVariant = nullptr;
	std::vector<SceneTree2D>& sceneTree2Ds = this->sceneStorage->getActiveScene().getSceneTree2Ds();
	SceneTree2D* sceneTree2D = nullptr;

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			sceneTree2D = &sceneTree2Ds.at(i);
	
	Entity2D& entity2D = sceneTree2D->getEntity2D(this->boundEntityID);
	std::vector<ComponentVariant>& componentVariants = sceneTree2D->getComponentVariants();

	for (int i = 0; i < componentVariants.size(); i++)
		if (componentVariants.at(i).getID() == entity2D.components.at(type))
			componentVariant = &componentVariants.at(i);

	return *componentVariant;
}