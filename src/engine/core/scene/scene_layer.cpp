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

#include "scene_layer.hpp"
#include "core/component.hpp"

#include <core/components/transform.hpp>

Omnia::SceneLayer::SceneLayer()
{
	this->collisionRegistry = std::shared_ptr<CollisionRegistry>(new CollisionRegistry());
	this->eventBus = std::shared_ptr<EventBus>(new EventBus());
	this->outputEventBus = std::shared_ptr<EventBus>(new EventBus());
	this->hapticSignalBuffer = std::shared_ptr<HapticSignalBuffer>(new HapticSignalBuffer());

	this->id = UIDGenerator::getNewUID();
	this->name = "SceneLayer (ID:" + std::to_string(this->id) + ")";
}

void Omnia::SceneLayer::addEntity(std::shared_ptr<Entity> entity)
{
	if (entity->parentID != 0)
		this->entities.at(entity->parentID)->childIDs.push_back(entity->id);

	this->startEntitiesQueue.emplace(entity->id);
	this->entities.emplace(entity->id, entity);
	this->lastEntityID = entity->id;
	this->setEntityName(entity->id, entity->name);
}

void Omnia::SceneLayer::addEmptyEntity()
{
	std::shared_ptr<Entity> emptyEntity(new Entity());
	this->addEntity(emptyEntity);
}

void Omnia::SceneLayer::setEntityName(EntityID entityID, std::string name)
{
	if (this->entityNames.count(name))
		name += "(Copy)";

	this->getEntity(entityID)->name = name;
	this->entityNames.emplace(name, entityID);
}

void Omnia::SceneLayer::addEntityTag(EntityID entityID, std::string tag)
{
	this->getEntity(entityID)->tags.push_back(tag);
	this->entityTags.emplace(tag, entityID);
}

void Omnia::SceneLayer::addComponent(EntityID entityID, std::shared_ptr<Component> component)
{
	component->setEntityID(entityID);
	this->components.push_back(component);
	std::string type = component->getType();
	std::shared_ptr<Entity> entity = this->entities.at(entityID);
	entity->componentIDs.emplace(type, component->getID());
	size_t lastIndex = this->components.size() - 1;

	if (this->componentIndexCaches.count(type) > 0)
	{
		this->componentIndexCaches.at(type).push_back(lastIndex);
	}
	else
	{
		std::vector<size_t> componentIndices;
		componentIndices.push_back(lastIndex);
		this->componentIndexCaches.emplace(type, componentIndices);
	}

	this->eventBus->publish(OMNIA_EVENT_COMPONENT_CHANGED);

	if (component->isRenderable())
	{
		entity->renderableComponentID = component->getID();
		this->renderOrderIndexCache.push_back(lastIndex);
		this->outputEventBus->publish(OMNIA_EVENT_RENDERABLE_COMPONENT_CHANGED);
	}

	component->setComponentHierarchy(this->getComponentHierarchy(component->getType(), component->getEntityID()));
}

void Omnia::SceneLayer::addComponentToLastEntity(std::shared_ptr<Component> component)
{
	this->addComponent(this->lastEntityID, component);
}

void Omnia::SceneLayer::removeEntity(EntityID entityID)
{
	if (this->entities.count(entityID) > 0)
	{
		std::unordered_map<std::string, ComponentID> entityComponentIDs = this->getEntity(entityID)->componentIDs;

		for (auto it = entityComponentIDs.begin(); it != entityComponentIDs.end(); it++)
			this->removeComponent(entityID, it->first);

		/* Remove the children */
		std::vector<EntityID> childIDs = this->getEntity(entityID)->childIDs;

		for (int i = 0; i < childIDs.size(); i++)
			this->removeEntity(childIDs.at(i));

		/* Remove the ID from the parent children list */
		std::shared_ptr<Entity> parentEntity = this->getEntity(this->getEntity(entityID)->parentID);

		for (auto it = parentEntity->childIDs.begin(); it != parentEntity->childIDs.end();)
		{
			if ((*it) == entityID)
			{
				it = parentEntity->childIDs.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		/* Remove the entity itself*/

		this->entities.erase(entityID);
	}
}

void Omnia::SceneLayer::removeComponent(EntityID entityID, std::string type)
{
	if (this->entities.count(entityID) > 0)
	{
		std::shared_ptr<Entity> entity = this->getEntity(entityID);

		if (entity->componentIDs.count(type) > 0)
		{
			ComponentID componentID = entity->componentIDs.at(type);
			entity->componentIDs.erase(type);

			/* Remove the component from the list. */

			for (auto it = this->components.begin(); it != this->components.end();)
			{
				if ((*it)->getID() == componentID)
				{
					if ((*it)->isRenderable())
						this->outputEventBus->publish(OMNIA_EVENT_RENDERABLE_COMPONENT_CHANGED);
					it = this->components.erase(it);
					this->eventBus->publish(OMNIA_EVENT_COMPONENT_CHANGED);
					break;
				}
				else
				{
					++it;
				}
			}

			/* Rebuild index caches */

			this->componentIndexCaches.clear();
			this->renderOrderIndexCache.clear();

			for (size_t i = 0; i < components.size(); i++)
			{
				std::shared_ptr<Component> component = components.at(i);
				this->componentIndexCaches.at(component->getType()).push_back(i);
				if (component->isRenderable())
					this->renderOrderIndexCache.push_back(i);
			}
		}
	}
}

std::vector<size_t> Omnia::SceneLayer::getRenderOrderIndexCache()
{
	return this->renderOrderIndexCache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnia::SceneLayer::getComponentIndexCaches()
{
	return this->componentIndexCaches;
}

void Omnia::SceneLayer::clearStartEntityQueue()
{
	while (!this->startEntitiesQueue.empty())
		this->startEntitiesQueue.pop();
}

void Omnia::SceneLayer::clearFinishEntityQueue()
{
	while (!this->finishEntitiesQueue.empty())
		this->finishEntitiesQueue.pop();
}

std::queue<Omnia::EntityID> Omnia::SceneLayer::getStartEntityQueue()
{
	return this->startEntitiesQueue;
}

std::queue<Omnia::EntityID> Omnia::SceneLayer::getFinishEntityQueue()
{
	return this->finishEntitiesQueue;
}

std::vector<std::shared_ptr<Omnia::Component>> Omnia::SceneLayer::getComponents()
{
	return this->components;
}

std::shared_ptr<Omnia::Entity> Omnia::SceneLayer::getEntity(EntityID entityID)
{
	return this->entities.at(entityID);
}

std::shared_ptr<Omnia::Entity> Omnia::SceneLayer::getEntityByName(std::string name)
{
	std::shared_ptr<Entity> entity(new Entity());

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second->name == name)
			return it->second;

	return entity;
}

std::shared_ptr<Omnia::Entity> Omnia::SceneLayer::getLastEntity()
{
	return this->entities.at(this->lastEntityID);
}

std::unordered_map<Omnia::EntityID, std::shared_ptr<Omnia::Entity>>& Omnia::SceneLayer::getEntities()
{
	return this->entities;
}

std::shared_ptr<Omnia::Component> Omnia::SceneLayer::getComponentByID(ComponentID componentID)
{
	std::shared_ptr<Component> component;

	for (int i = 0; i < this->components.size(); i++)
	{
		std::shared_ptr<Component> currentComponent = this->components.at(i);
		if (currentComponent->getID() == componentID)
		{
			component = currentComponent;
			break;
		}
	}

	return component;
}

std::shared_ptr<Omnia::Component> Omnia::SceneLayer::getComponent(std::string type, EntityID entityID)
{
	std::shared_ptr<Entity> entity = this->getEntity(entityID);
	std::shared_ptr<Component> component;

	if (entity->componentIDs.count(type) > 0)
		component = this->getComponentByID(entity->componentIDs.at(type));

	return component;
}

std::vector<std::shared_ptr<Omnia::Component>> Omnia::SceneLayer::getComponentHierarchy(std::string type, EntityID entityID)
{
	EntityID currentEntityID = entityID;
	std::vector<std::shared_ptr<Component>> componentHierarchy;

	do
	{
		std::shared_ptr<Component> component = this->getComponent(type, currentEntityID);
		componentHierarchy.push_back(component);
		currentEntityID = this->getEntity(currentEntityID)->parentID;
	} while (currentEntityID != 0);

	return componentHierarchy;
}

std::shared_ptr<Omnia::CollisionRegistry> Omnia::SceneLayer::getCollisionRegistry()
{
	return this->collisionRegistry;
}

std::shared_ptr<Omnia::EventBus> Omnia::SceneLayer::getEventBus()
{
	return this->eventBus;
}

std::shared_ptr<Omnia::EventBus> Omnia::SceneLayer::getOutputEventBus()
{
	return this->outputEventBus;
}

Omnia::SceneLayerID Omnia::SceneLayer::getID()
{
	return this->id;
}

std::string Omnia::SceneLayer::getName()
{
	return this->name;
}

std::shared_ptr<Omnia::HapticSignalBuffer> Omnia::SceneLayer::getHapticSignalBuffer()
{
	return this->hapticSignalBuffer;
}