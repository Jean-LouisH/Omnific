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

#include "cpp_entity_context.hpp"
#include <core/singletons/os/os.hpp>

Omnia::CPPEntityContext* Omnia::CPPEntityContext::instance = nullptr;

void Omnia::CPPEntityContext::bindEntity(SceneLayerID sceneLayerID, EntityID entityID)
{
	getInstance()->boundSceneLayerID = sceneLayerID;
	getInstance()->boundEntityID = entityID;
}

bool Omnia::CPPEntityContext::hasComponent(std::string type)
{
	return getInstance()->getSceneLayer()->getEntity(getInstance()->boundEntityID)->componentIDs.count(type) > 0;
}

std::shared_ptr<Omnia::Entity> Omnia::CPPEntityContext::getEntity()
{
	return getInstance()->getSceneLayer()->getEntity(getInstance()->boundEntityID);
}

std::shared_ptr<Omnia::Scene> Omnia::CPPEntityContext::getScene()
{
	return SceneStorage::getActiveScene();
}

std::shared_ptr<Omnia::SceneLayer> Omnia::CPPEntityContext::getSceneLayer()
{
	return getInstance()->getScene()->getSceneLayers().at(getInstance()->boundSceneLayerID);
}

std::shared_ptr<Omnia::Component> Omnia::CPPEntityContext::getComponent(std::string type)
{
	std::shared_ptr<Component> component(new Component());

	std::shared_ptr<Entity> entity = getInstance()->getSceneLayer()->getEntity(getInstance()->boundEntityID);
	std::vector<std::shared_ptr<Component>>& components = getInstance()->getSceneLayer()->getComponents();

	for (int i = 0; i < components.size(); i++)
		if (components.at(i)->getID() == entity->componentIDs.at(type))
			component = components.at(i);

	return component;
}

float Omnia::CPPEntityContext::getTimeDelta()
{
	return OS::getProfiler().getTimer("update_frame")->getDeltaInSeconds();
}

Omnia::CPPEntityContext* Omnia::CPPEntityContext::getInstance()
{
	if (instance == nullptr)
		instance = new CPPEntityContext();
	return instance;
}