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

#include "scene.hpp"
#include "application/scripting/virtual_machine/script_call_batch.hpp"

Esi::Scene::Scene()
{
	Transform* transform = new Transform();
	ComponentVariant componentVariant;

	this->ID = IDCounter::getNewSceneID();
	componentVariant.setTo(transform);
	this->addEmptyEntity();
	this->addComponentToLastEntity(componentVariant);
	this->dummyEntityID = this->getLastEntity().ID;
}

void Esi::Scene::addEntity(Entity entity)
{
	this->startEntitiesQueue.emplace(entity.ID);
	this->entities.emplace(entity.ID, entity);
	this->lastEntityID = entity.ID;
}

void Esi::Scene::addEmptyEntity()
{
	Entity emptyEntity;
	this->addEntity(emptyEntity);
}

void Esi::Scene::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.setEntityID(entityID);
	this->componentVariants.push_back(componentVariant);
	ComponentVariant::Type type = componentVariant.getType();
	this->entities.at(entityID).components.emplace(type, componentVariant.getID());
	size_t lastIndex = this->componentVariants.size() - 1;

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

	if (componentVariant.isRenderable())
		this->renderOrderIndexCache.push_back(this->componentVariants.size() - 1);
}

void Esi::Scene::addComponentToLastEntity(ComponentVariant componentVariant)
{
	this->addComponent(this->lastEntityID, componentVariant);
}

void Esi::Scene::removeEntity(EntityID entityID)
{
	for (int i = ComponentVariant::Type::BEHAVIOUR_TREE;
		i < ComponentVariant::Type::UI_TREE;
		i++)
	{
		this->removeComponent(entityID, (ComponentVariant::Type)i);
	}

	std::vector<EntityID> childIDs = this->getEntity(entityID).childIDs;

	for (int i = 0; i < childIDs.size(); i++)
		this->getEntity(childIDs.at(i)).parentID = DUMMY_ENTITY;

	EntityID parentID = this->getEntity(entityID).parentID;
	std::vector<EntityID> parentChildIDs = this->getEntity(parentID).childIDs;
}

void Esi::Scene::removeComponent(EntityID entityID, ComponentVariant::Type type)
{
	Entity& Entity = this->getEntity(entityID);
	ComponentID componentID = Entity.components.at(type);

	Entity.components.erase(type);

	for (auto it = this->componentVariants.begin(); it != this->componentVariants.end();)
		if (it->getID() == componentID)
			it = this->componentVariants.erase(it);
		else
			++it;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnStartCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;

		if (!this->startEntitiesQueue.empty())
		{
			if (entity.ID == this->startEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = entity.scripts;
				scriptCallBatch.entityID = entity.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->startEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnInputCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity.scripts;
		scriptCallBatch.entityID = entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnFrameCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity.scripts;
		scriptCallBatch.entityID = entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnComputeCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity.scripts;
		scriptCallBatch.entityID = entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnOutputCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity.scripts;
		scriptCallBatch.entityID = entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Esi::ScriptCallBatch> Esi::Scene::generateOnFinishBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
	{
		Entity entity = it->second;

		if (!this->finishEntitiesQueue.empty())
		{
			if (entity.ID == this->finishEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = entity.scripts;
				scriptCallBatch.entityID = entity.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->finishEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

std::vector<size_t> Esi::Scene::getRenderOrderIndexCache()
{
	return this->renderOrderIndexCache;
}

std::unordered_map<Esi::ComponentVariant::Type, std::vector<size_t>> Esi::Scene::getComponentIndexCaches()
{
	return this->componentIndexCaches;
}

std::vector<Esi::ComponentVariant>& Esi::Scene::getComponentVariants()
{
	return this->componentVariants;
}

Esi::Transform& Esi::Scene::getEntityTransform(EntityID entityID)
{
	std::vector<size_t> transformIndices = this->componentIndexCaches.at(ComponentVariant::Type::TRANSFORM);
	Transform* transform = this->componentVariants.at(
		transformIndices.at(0)).getTransform();

	for (int i = 0; i < transformIndices.size(); i++)
		if (this->componentVariants.at(transformIndices.at(i)).getEntityID() == entityID)
			transform = this->componentVariants.at(transformIndices.at(i)).getTransform();

	return *transform;
}

Esi::Entity& Esi::Scene::getEntity(EntityID entityID)
{
	return this->entities.at(entityID);
}

Esi::Entity& Esi::Scene::getEntityByName(std::string name)
{
	Entity* Entity = nullptr;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second.name == name)
			return it->second;

	return *Entity;
}

Esi::Entity& Esi::Scene::getLastEntity()
{
	return this->entities.at(this->lastEntityID);
}

std::unordered_map<Esi::EntityID, Esi::Entity>& Esi::Scene::getEntities()
{
	return this->entities;
}

Esi::ComponentVariant& Esi::Scene::getComponent(ComponentID componentID)
{
	ComponentVariant* componentVariant = nullptr;

	for (int i = 0; i < this->componentVariants.size(); i++)
	{
		ComponentVariant& currentComponentVariant = this->componentVariants.at(i);
		if (currentComponentVariant.getID() == componentID)
			componentVariant = &currentComponentVariant;
	}

	return *componentVariant;
}

Esi::Entity::SpatialDimension Esi::Scene::getComponentSpatialDimension(ComponentID componentID)
{
	return this->getEntity(this->getComponent(componentID).getEntityID()).spatialDimension;
}

Esi::EventBus& Esi::Scene::getEventBus()
{
	return this->eventBus;
}

Esi::SceneID Esi::Scene::getID()
{
	return this->ID;
}

Esi::AssetCache& Esi::Scene::getAssetCache()
{
	return this->assetCache;
}

Esi::HapticSignalBuffer& Esi::Scene::getHapticSignalBuffer()
{
	return this->hapticSignalBuffer;
}

void Esi::Scene::unload()
{
	this->assetCache.deleteAll();
}

Esi::Entity& Esi::Scene::getDummyEntity()
{
	return this->getEntity(this->dummyEntityID);
}