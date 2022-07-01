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

#include "scene_tree.hpp"
#include "component.hpp"

Omnia::SceneTree::SceneTree()
{
	this->collisionRegistry = std::shared_ptr<CollisionRegistry>(new CollisionRegistry());
	this->eventBus = std::shared_ptr<EventBus>(new EventBus());
	this->hapticSignalBuffer = std::shared_ptr<HapticSignalBuffer>(new HapticSignalBuffer());
	this->componentPropertyPool = std::shared_ptr<ComponentPropertyPool>(new ComponentPropertyPool());

	this->id = UIDGenerator::getNewUID();
	this->name = "SceneTree (ID:" + std::to_string(this->id) + ")";
}

void Omnia::SceneTree::addEntity(std::shared_ptr<Entity> entity)
{
	if (entity->parentID != 0)
		this->entities.at(entity->parentID)->childIDs.push_back(entity->id);

	this->startEntitiesQueue.emplace(entity->id);
	this->entities.emplace(entity->id, entity);
	this->lastEntityID = entity->id;
	this->setEntityName(entity->id, entity->name);
}

void Omnia::SceneTree::addEmptyEntity()
{
	std::shared_ptr<Entity> emptyEntity(new Entity());
	this->addEntity(emptyEntity);
}

void Omnia::SceneTree::setEntityName(EntityID entityID, std::string name)
{
	if (this->entityNames.count(name))
		name += "(Copy)";

	this->getEntity(entityID)->name = name;
	this->entityNames.emplace(name, entityID);
}

void Omnia::SceneTree::addEntityTag(EntityID entityID, std::string tag)
{
	this->getEntity(entityID)->tags.push_back(tag);
	this->entityTags.emplace(tag, entityID);
}

void Omnia::SceneTree::addComponent(EntityID entityID, std::shared_ptr<Component> component)
{
	if (component->isType(DirectionalLight::TYPE_STRING))
	{
		if (this->directionalLightCount >= this->allowableDirectionalLights)
			return;
		else
			this->directionalLightCount++;
	}
	else if (component->isType(OmnidirectionalLight::TYPE_STRING) ||
		component->isType(SpotLight::TYPE_STRING))
	{
		if (this->localLightCount >= this->allowableLocalLights)
			return;
		else
			this->localLightCount++;
	}

	component->setEntityID(entityID);
	component->setComponentPropertyPool(this->componentPropertyPool);
	this->components.push_back(component);
	std::string type = component->getType();
	this->entities.at(entityID)->componentIDs.emplace(type, component->getID());
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

	if (component->isRenderable())
	{
		this->renderOrderIndexCache.push_back(lastIndex);
		this->hasRenderableComponentsChanged = true;
	}
}

void Omnia::SceneTree::addComponentToLastEntity(std::shared_ptr<Component> component)
{
	this->addComponent(this->lastEntityID, component);
}

void Omnia::SceneTree::removeEntity(EntityID entityID)
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

void Omnia::SceneTree::removeComponent(EntityID entityID, std::string type)
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
						this->hasRenderableComponentsChanged = true;
					it = this->components.erase(it);
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

std::vector<size_t> Omnia::SceneTree::getRenderOrderIndexCache()
{
	return this->renderOrderIndexCache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnia::SceneTree::getComponentIndexCaches()
{
	return this->componentIndexCaches;
}

void Omnia::SceneTree::clearStartEntityQueue()
{
	while (!this->startEntitiesQueue.empty())
		this->startEntitiesQueue.pop();
}

void Omnia::SceneTree::clearFinishEntityQueue()
{
	while (!this->finishEntitiesQueue.empty())
		this->finishEntitiesQueue.pop();
}

std::queue<Omnia::EntityID> Omnia::SceneTree::getStartEntityQueue()
{
	return this->startEntitiesQueue;
}

std::queue<Omnia::EntityID> Omnia::SceneTree::getFinishEntityQueue()
{
	return this->finishEntitiesQueue;
}

std::vector<std::shared_ptr<Omnia::Component>> Omnia::SceneTree::getComponents()
{
	return this->components;
}

std::shared_ptr<Omnia::Transform> Omnia::SceneTree::getEntityTransform(EntityID entityID)
{
	return this->getComponent<Transform>(entityID);
}

std::shared_ptr<Omnia::Entity> Omnia::SceneTree::getEntity(EntityID entityID)
{
	return this->entities.at(entityID);
}

std::shared_ptr<Omnia::Entity> Omnia::SceneTree::getEntityByName(std::string name)
{
	std::shared_ptr<Entity> entity(new Entity());

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second->name == name)
			return it->second;

	return entity;
}

std::shared_ptr<Omnia::Entity> Omnia::SceneTree::getLastEntity()
{
	return this->entities.at(this->lastEntityID);
}

std::unordered_map<Omnia::EntityID, std::shared_ptr<Omnia::Entity>>& Omnia::SceneTree::getEntities()
{
	return this->entities;
}

std::shared_ptr<Omnia::Component> Omnia::SceneTree::getComponent(ComponentID componentID)
{
	std::shared_ptr<Component> component(new Component());

	for (int i = 0; i < this->components.size(); i++)
	{
		std::shared_ptr<Component> currentComponent = this->components.at(i);
		if (currentComponent->getID() == componentID)
			component = currentComponent;
	}

	return component;
}

std::shared_ptr<Omnia::CollisionRegistry> Omnia::SceneTree::getCollisionRegistry()
{
	return this->collisionRegistry;
}

std::shared_ptr<Omnia::EventBus> Omnia::SceneTree::getEventBus()
{
	return this->eventBus;
}

bool Omnia::SceneTree::getHasShadersChanged()
{
	return false;
}

bool Omnia::SceneTree::getHasScriptsChanged()
{
	return false;
}

Omnia::SceneTreeID Omnia::SceneTree::getID()
{
	return this->id;
}

std::string Omnia::SceneTree::getName()
{
	return this->name;
}

std::shared_ptr<Omnia::HapticSignalBuffer> Omnia::SceneTree::getHapticSignalBuffer()
{
	return this->hapticSignalBuffer;
}