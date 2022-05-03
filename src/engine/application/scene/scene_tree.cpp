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
#include "application/scripting/python_vm/script_call_batch.hpp"

Omnia::SceneTree::SceneTree()
{
	this->collisionRegistry = std::shared_ptr<CollisionRegistry>(new CollisionRegistry());
	this->eventBus = std::shared_ptr<EventBus>(new EventBus());
	this->hapticSignalBuffer = std::shared_ptr<HapticSignalBuffer>(new HapticSignalBuffer());
	this->componentPropertyPool = std::shared_ptr<ComponentPropertyPool>(new ComponentPropertyPool());

	this->id = UIDGenerator::getNewUID();
	this->name = "SceneTree (ID:" + std::to_string(this->id) + ")";
}

void Omnia::SceneTree::addEntity(Entity entity)
{
	if (entity.parentID != 0)
		this->entities.at(entity.parentID).childIDs.push_back(entity.id);

	this->startEntitiesQueue.emplace(entity.id);
	this->entities.emplace(entity.id, entity);
	this->lastEntityID = entity.id;
	this->setEntityName(entity.id, entity.name);
}

void Omnia::SceneTree::addEmptyEntity()
{
	Entity emptyEntity;
	this->addEntity(emptyEntity);
}

void Omnia::SceneTree::setEntityName(EntityID entityID, std::string name)
{
	if (this->entityNames.count(name))
		name += "(Copy)";

	this->getEntity(entityID).name = name;
	this->entityNames.emplace(name, entityID);
}

void Omnia::SceneTree::addEntityTag(EntityID entityID, std::string tag)
{
	this->getEntity(entityID).tags.push_back(tag);
	this->entityTags.emplace(tag, entityID);
}

void Omnia::SceneTree::addComponent(EntityID entityID, std::shared_ptr<Component> component)
{
	component->setEntityID(entityID);
	component->setComponentPropertyPool(this->componentPropertyPool);
	this->components.push_back(component);
	std::string type = component->getType();
	this->entities.at(entityID).componentIDs.emplace(type, component->getID());
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
		std::unordered_map<std::string, ComponentID> entityComponentIDs = this->getEntity(entityID).componentIDs;

		for (auto it = entityComponentIDs.begin(); it != entityComponentIDs.end(); it++)
			this->removeComponent(entityID, it->first);

		/* Remove the children */
		std::vector<EntityID> childIDs = this->getEntity(entityID).childIDs;

		for (int i = 0; i < childIDs.size(); i++)
			this->removeEntity(childIDs.at(i));

		/* Remove the ID from the parent children list */
		Entity& parentEntity = this->getEntity(this->getEntity(entityID).parentID);

		for (auto it = parentEntity.childIDs.begin(); it != parentEntity.childIDs.end();)
		{
			if ((*it) == entityID)
			{
				it = parentEntity.childIDs.erase(it);
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
		Entity& entity = this->getEntity(entityID);

		if (entity.componentIDs.count(type) > 0)
		{
			ComponentID componentID = entity.componentIDs.at(type);
			entity.componentIDs.erase(type);

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

std::vector<Omnia::ScriptCallBatch> Omnia::SceneTree::generateCallBatches(CallType callType)
{
	std::vector<ScriptCallBatch> scriptCallBatches;
	std::queue<EntityID>* entityQueue = nullptr;
	std::vector<std::shared_ptr<ScriptCollection>> scriptCollections = this->getComponentsByType<ScriptCollection>();
	size_t scriptCollectionsCount = scriptCollections.size();

	if (callType == CallType::START || callType == CallType::FINISH)
	{
		if (callType == CallType::START)
			entityQueue = &this->startEntitiesQueue;
		else if (callType == CallType::FINISH)
			entityQueue = &this->finishEntitiesQueue;

		while (!entityQueue->empty())
		{
			Entity& entity = this->getEntity(entityQueue->front());
			std::shared_ptr<ScriptCollection> scriptCollection = this->getComponent<ScriptCollection>(entity.id);
			if (scriptCollection != nullptr)
			{
				std::vector<std::string> scriptNames;

				for (size_t j = 0; j < scriptCollection->scripts.size(); j++)
					scriptNames.push_back(scriptCollection->scripts.at(j)->getName());

				scriptCallBatches.push_back({ scriptNames, this->id, scriptCollection->getEntityID() });
			}
			entityQueue->pop();
		}
	}
	else if (callType == CallType::UPDATE)
	{
		for (size_t i = 0; i < scriptCollectionsCount; i++)
		{
			std::shared_ptr<ScriptCollection> scriptCollection = scriptCollections.at(i);
			std::vector<std::string> scriptNames;

			for (size_t j = 0; j < scriptCollection->scripts.size(); j++)
				scriptNames.push_back(scriptCollection->scripts.at(j)->getName());

			scriptCallBatches.push_back({ scriptNames, this->id, scriptCollection->getEntityID() });
		}
	}

	return scriptCallBatches;
}

std::vector<size_t> Omnia::SceneTree::getRenderOrderIndexCache()
{
	return this->renderOrderIndexCache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnia::SceneTree::getComponentIndexCaches()
{
	return this->componentIndexCaches;
}

std::vector<std::shared_ptr<Omnia::Component>> Omnia::SceneTree::getComponents()
{
	return this->components;
}

std::shared_ptr<Omnia::Transform> Omnia::SceneTree::getEntityTransform(EntityID entityID)
{
	return this->getComponent<Transform>(entityID);
}

Omnia::Entity& Omnia::SceneTree::getEntity(EntityID entityID)
{
	return this->entities.at(entityID);
}

Omnia::Entity& Omnia::SceneTree::getEntityByName(std::string name)
{
	Entity* entity = nullptr;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second.name == name)
			return it->second;

	return *entity;
}

Omnia::Entity& Omnia::SceneTree::getLastEntity()
{
	return this->entities.at(this->lastEntityID);
}

std::unordered_map<Omnia::EntityID, Omnia::Entity>& Omnia::SceneTree::getEntities()
{
	return this->entities;
}

std::shared_ptr<Omnia::Component> Omnia::SceneTree::getComponent(ComponentID componentID)
{
	std::shared_ptr<Component> component = std::make_shared<Component>();

	for (int i = 0; i < this->components.size(); i++)
	{
		std::shared_ptr<Component> currentComponent = this->components.at(i);
		if (currentComponent->getID() == componentID)
			component = currentComponent;
	}

	return component;
}

Omnia::CollisionRegistry& Omnia::SceneTree::getCollisionRegistry()
{
	return *this->collisionRegistry;
}

Omnia::EventBus& Omnia::SceneTree::getEventBus()
{
	return *this->eventBus;
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

Omnia::HapticSignalBuffer& Omnia::SceneTree::getHapticSignalBuffer()
{
	return *this->hapticSignalBuffer;
}