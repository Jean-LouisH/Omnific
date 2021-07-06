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

Lilliputian::Scene::Scene()
{
	Transform* transform2D = new Transform();
	ComponentVariant componentVariant;

	this->ID = IDCounter::getNewSceneID();
	componentVariant.setTo(transform2D);
	this->addEmptyEntity();
	this->addComponentToLastEntity(componentVariant);
	this->dummyEntityID = this->getLastEntity().ID;
}

void Lilliputian::Scene::addEntity(Entity Entity)
{
	this->startEntitiesQueue.emplace(Entity.ID);
	this->entities2D.emplace(Entity.ID, Entity);
	this->lastEntityID = Entity.ID;
}

void Lilliputian::Scene::addEmptyEntity()
{
	Entity emptyEntity;
	this->addEntity(emptyEntity);
}

void Lilliputian::Scene::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.setEntityID(entityID);
	this->componentVariants.push_back(componentVariant);
	this->entities2D.at(entityID).components.emplace(componentVariant.getType(), componentVariant.getID());

	if (componentVariant.getType() == ComponentVariant::Type::TRANSFORM)
		this->transform2DIndexCache.push_back(this->componentVariants.size() - 1);
	else if (componentVariant.getType() == ComponentVariant::Type::CAMERA)
		if (componentVariant.getCamera2D()->getIsStreaming())
			this->currentCameraID = componentVariant.getID();
}

void Lilliputian::Scene::addComponentToLastEntity(ComponentVariant componentVariant)
{
	this->addComponent(this->lastEntityID, componentVariant);
}

void Lilliputian::Scene::removeEntity(EntityID entityID)
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

void Lilliputian::Scene::removeComponent(EntityID entityID, ComponentVariant::Type type)
{
	Entity& Entity = this->getEntity(entityID);
	ComponentID componentID = Entity.components.at(type);

	Entity.components.erase(type);

	if (componentID == this->currentCameraID)
		this->currentCameraID = -1;

	for (auto it = this->componentVariants.begin(); it != this->componentVariants.end();)
		if (it->getID() == componentID)
			it = this->componentVariants.erase(it);
		else
			++it;
}

void Lilliputian::Scene::changeCurrentCamera(Lilliputian::ComponentID newCurrentCameraID)
{
	for (int i = 0; i < this->componentVariants.size(); i++)
	{
		if (this->componentVariants.at(i).getID() == newCurrentCameraID)
		{
			if (this->componentVariants.at(i).getType() == ComponentVariant::Type::CAMERA)
				this->currentCameraID = newCurrentCameraID;

			break;
		}
	}
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnStartCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;

		if (!this->startEntitiesQueue.empty())
		{
			if (Entity.ID == this->startEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = Entity.scripts;
				scriptCallBatch.entityID = Entity.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->startEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnInputCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = Entity.scripts;
		scriptCallBatch.entityID = Entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnFrameCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = Entity.scripts;
		scriptCallBatch.entityID = Entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnComputeCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = Entity.scripts;
		scriptCallBatch.entityID = Entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnLateCallBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = Entity.scripts;
		scriptCallBatch.entityID = Entity.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

std::vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnFinalBatches()
{
	std::vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity Entity = it->second;

		if (!this->finishEntitiesQueue.empty())
		{
			if (Entity.ID == this->finishEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = Entity.scripts;
				scriptCallBatch.entityID = Entity.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->finishEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

Lilliputian::ComponentID Lilliputian::Scene::getCurrentCameraID()
{
	return this->currentCameraID;
}

std::vector<Lilliputian::ComponentVariant>& Lilliputian::Scene::getComponentVariants()
{
	return this->componentVariants;
}

Lilliputian::Transform& Lilliputian::Scene::getEntityTransform(EntityID entityID)
{
	Transform* transform2D = this->componentVariants.at(this->transform2DIndexCache.at(0)).getTransform2D();

	for (int i = 0; i < this->transform2DIndexCache.size(); i++)
		if (this->componentVariants.at(this->transform2DIndexCache.at(i)).getEntityID() == entityID)
			transform2D = this->componentVariants.at(this->transform2DIndexCache.at(i)).getTransform2D();

	return *transform2D;
}

Lilliputian::Entity& Lilliputian::Scene::getEntity(EntityID entityID)
{
	return this->entities2D.at(entityID);
}

Lilliputian::Entity& Lilliputian::Scene::getEntityByName(std::string name)
{
	Entity* Entity = nullptr;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
		if (it->second.name == name)
			return it->second;

	return *Entity;
}

Lilliputian::Entity& Lilliputian::Scene::getLastEntity()
{
	return this->entities2D.at(this->lastEntityID);
}

std::map<Lilliputian::EntityID, Lilliputian::Entity>& Lilliputian::Scene::getEntities()
{
	return this->entities2D;
}

Lilliputian::EventBus& Lilliputian::Scene::getEventBus()
{
	return this->eventBus;
}

Lilliputian::SceneID Lilliputian::Scene::getID()
{
	return this->ID;
}

Lilliputian::AssetCache& Lilliputian::Scene::getAssetCache()
{
	return this->assetCache;
}

Lilliputian::HapticSignalBuffer& Lilliputian::Scene::getHapticSignalBuffer()
{
	return this->hapticSignalBuffer;
}

void Lilliputian::Scene::unload()
{
	this->assetCache.deleteAll();
}

Lilliputian::Entity& Lilliputian::Scene::getDummyEntity()
{
	return this->getEntity(this->dummyEntityID);
}