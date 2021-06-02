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

#include "scene_tree_2d.hpp"
#include "game/scripting/virtual_machine/script_call_batch.hpp"
#include "id_counter.hpp"

Lilliputian::SceneTree2D::SceneTree2D()
{
	Transform2D* transform2D = new Transform2D();
	ComponentVariant componentVariant;

	this->ID = IDCounter::getNewID();
	componentVariant.setToTransform2D(transform2D);
	this->addEmptyEntity2D();
	this->addComponentToLastEntity(componentVariant);
	this->dummyEntityID = this->getLastEntity2D().ID;
}

void Lilliputian::SceneTree2D::addEntity2D(Entity2D entity2D)
{
	this->startEntitiesQueue.emplace(entity2D.ID);
	this->entities2D.emplace(entity2D.ID, entity2D);
	this->lastEntityID = entity2D.ID;
}

void Lilliputian::SceneTree2D::addEmptyEntity2D()
{
	Entity2D emptyEntity2D;
	this->addEntity2D(emptyEntity2D);
}

void Lilliputian::SceneTree2D::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.setEntityID(entityID);
	this->componentVariants.push_back(componentVariant);
	this->entities2D.at(entityID).components.emplace(componentVariant.getType(), componentVariant.getID());

	if (componentVariant.getType() == ComponentVariant::Type::TRANSFORM_2D)
		this->transform2DIndexCache.push_back(this->componentVariants.size() - 1);
	else if (componentVariant.getType() == ComponentVariant::Type::CAMERA_2D)
		if (componentVariant.getCamera2D()->getIsStreaming())
			this->currentCameraID = componentVariant.getID();
}

void Lilliputian::SceneTree2D::addComponentToLastEntity(ComponentVariant componentVariant)
{
	this->addComponent(this->lastEntityID, componentVariant);
}

void Lilliputian::SceneTree2D::removeEntity2D(EntityID entityID)
{
	for (int i = ComponentVariant::Type::AI_BEHAVIOUR_TREE;
		i < ComponentVariant::Type::UI_TREE; 
		i++)
	{
		this->removeComponent(entityID, (ComponentVariant::Type)i);
	}

	Vector<EntityID> childIDs = this->getEntity2D(entityID).childIDs;

	for (int i = 0; i < childIDs.size(); i++)
		this->getEntity2D(childIDs.at(i)).parentID = DUMMY_ENTITY;

	EntityID parentID = this->getEntity2D(entityID).parentID;
	Vector<EntityID> parentChildIDs = this->getEntity2D(parentID).childIDs;
}

void Lilliputian::SceneTree2D::removeComponent(EntityID entityID, ComponentVariant::Type type)
{
	Entity2D& entity2D = this->getEntity2D(entityID);
	ComponentID componentID = entity2D.components.at(type);

	entity2D.components.erase(type);

	if (componentID == this->currentCameraID)
		this->currentCameraID = -1;

	for (auto it = this->componentVariants.begin(); it != this->componentVariants.end();)
		if (it->getID() == componentID)
			it = this->componentVariants.erase(it);
		else
			++it;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnStartCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it !=  this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;

		if (!this->startEntitiesQueue.empty())
		{
			if (entity2D.ID == this->startEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = entity2D.scripts;
				scriptCallBatch.entityID = entity2D.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->startEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnInputCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity2D.scripts;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnFrameCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity2D.scripts;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnComputeCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity2D.scripts;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnLateCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scripts = entity2D.scripts;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnFinalCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
	{
		Entity2D entity2D = it->second;

		if (!this->finishEntitiesQueue.empty())
		{
			if (entity2D.ID == this->finishEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scripts = entity2D.scripts;
				scriptCallBatch.entityID = entity2D.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->finishEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

Lilliputian::ComponentID Lilliputian::SceneTree2D::getCurrentCameraID()
{
	return this->currentCameraID;
}

Lilliputian::Vector<Lilliputian::ComponentVariant>& Lilliputian::SceneTree2D::getComponentVariants()
{
	return this->componentVariants;
}

Lilliputian::Transform2D& Lilliputian::SceneTree2D::getEntityTransform(EntityID entityID)
{
	Transform2D* transform2D = this->componentVariants.at(this->transform2DIndexCache.at(0)).getTransform2D();

	for (int i = 0; i < this->transform2DIndexCache.size(); i++)
		if (this->componentVariants.at(this->transform2DIndexCache.at(i)).getEntityID() == entityID)
			transform2D = this->componentVariants.at(this->transform2DIndexCache.at(i)).getTransform2D();

	return *transform2D;
}

Lilliputian::Entity2D& Lilliputian::SceneTree2D::getEntity2D(EntityID entityID)
{
	return this->entities2D.at(entityID);
}

Lilliputian::Entity2D& Lilliputian::SceneTree2D::getEntity2DByName(String name)
{
	Entity2D* entity2D = nullptr;

	for (auto it = this->entities2D.begin(); it != this->entities2D.end(); it++)
		if (it->second.name == name)
			return it->second;

	return *entity2D;
}

Lilliputian::Entity2D& Lilliputian::SceneTree2D::getLastEntity2D()
{
	return this->entities2D.at(this->lastEntityID);
}

Lilliputian::Map<Lilliputian::EntityID, Lilliputian::Entity2D>& Lilliputian::SceneTree2D::getEntity2Ds()
{
	return this->entities2D;
}

Lilliputian::EventQueue& Lilliputian::SceneTree2D::getEventQueue()
{
	return this->eventQueue;
}

Lilliputian::SceneTreeID Lilliputian::SceneTree2D::getID()
{
	return this->ID;
}

void Lilliputian::SceneTree2D::unload()
{
	for (int i = 0; i < this->componentVariants.size(); i++)
		this->componentVariants.at(i).unloadImage();

	this->componentVariants.clear();
	this->entities2D.clear();
}

Lilliputian::Entity2D& Lilliputian::SceneTree2D::getDummyEntity2D()
{
	return this->getEntity2D(this->dummyEntityID);
}