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

#include "script_context.hpp"
#include <asset_cache.hpp>
#include <os/os.hpp>

Omnia::ScriptContext* Omnia::ScriptContext::instance = nullptr;

void Omnia::ScriptContext::setSceneStorage(std::shared_ptr<SceneStorage> sceneStorage)
{
	getInstance()->sceneStorage = sceneStorage;
}

void Omnia::ScriptContext::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	getInstance()->boundSceneTreeID = sceneTreeID;
	getInstance()->boundEntityID = entityID;
}

bool Omnia::ScriptContext::hasComponent(std::string type)
{
	return getInstance()->getSceneTree().getEntity(getInstance()->boundEntityID)->componentIDs.count(type) > 0;
}

std::shared_ptr<Omnia::Asset> Omnia::ScriptContext::loadAsset(std::string type, std::string filepath)
{
	std::shared_ptr<Asset> asset;
	std::shared_ptr<Image> image;

	if (type == Image::TYPE_STRING)
	{
		std::string absoluteFilepath = OS::getFileAccess().getDataDirectoryPath() + filepath;
		if (AssetCache::exists(absoluteFilepath))
		{
			asset = AssetCache::fetch(absoluteFilepath);
		}
		else
		{
			image = std::shared_ptr<Image>(new Image(absoluteFilepath));
			asset = std::static_pointer_cast<Asset>(image);
			AssetCache::store(asset);
		}
	}

	return asset;
}

std::shared_ptr<Omnia::Image> Omnia::ScriptContext::loadImage(std::string filepath)
{
	return std::dynamic_pointer_cast<Image>(getInstance()->loadAsset(Image::TYPE_STRING, filepath));
}

Omnia::Entity& Omnia::ScriptContext::getEntity()
{
	return *getInstance()->getSceneTree().getEntity(getInstance()->boundEntityID);
}

Omnia::Scene& Omnia::ScriptContext::getScene()
{
	return *getInstance()->sceneStorage->getActiveScene();
}

Omnia::SceneTree& Omnia::ScriptContext::getSceneTree()
{
	return *getInstance()->getScene().getSceneTrees().at(getInstance()->boundSceneTreeID);
}

Omnia::Component& Omnia::ScriptContext::getComponent(std::string type)
{
	std::shared_ptr<Component> component(new Component());

	std::shared_ptr<Entity> entity = getInstance()->getSceneTree().getEntity(getInstance()->boundEntityID);
	std::vector<std::shared_ptr<Component>>& components = getInstance()->getSceneTree().getComponents();

	for (int i = 0; i < components.size(); i++)
		if (components.at(i)->getID() == entity->componentIDs.at(type))
			component = components.at(i);

	return *component;
}

Omnia::ScriptContext* Omnia::ScriptContext::getInstance()
{
	if (instance == nullptr)
		instance = new ScriptContext();
	return instance;
}