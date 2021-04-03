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
#include "game/virtual_machine/script_call_batch.hpp"

Lilliputian::SceneTree2D::SceneTree2D()
{
	this->dummyTransform2D = new Transform2D();
}

void Lilliputian::SceneTree2D::addEntity2D(Entity2D entity2D)
{
	this->startEntitiesQueue.emplace(entity2D.ID);
	this->entities2D.emplace(entity2D.ID, entity2D);
}

void Lilliputian::SceneTree2D::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.entityID = entityID;
	this->componentVariants.push_back(componentVariant);
	this->entities2D.at(entityID).addComponent(componentVariant.type, this->componentVariants.size() - 1);

	if (componentVariant.type == ComponentVariant::COMPONENT_TYPE_TRANSFORM_2D)
		this->transform2DIndexCache.push_back(this->componentVariants.size() - 1);
	else if (componentVariant.type == ComponentVariant::COMPONENT_TYPE_CAMERA_2D)
		if (componentVariant.camera2D->getIsStreaming())
			this->currentCamera = this->componentVariants.size() - 1;
}

void Lilliputian::SceneTree2D::addScript(EntityID entityID, String scriptPath)
{
	this->entities2D.at(entityID).addScript(scriptPath);
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnStartCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);

		if (!this->startEntitiesQueue.empty())
		{
			if (entity2D.ID == this->startEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scriptPaths = entity2D.scriptPaths;
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

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scriptPaths = entity2D.scriptPaths;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnFrameCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scriptPaths = entity2D.scriptPaths;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnComputeCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scriptPaths = entity2D.scriptPaths;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnLateCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);
		ScriptCallBatch scriptCallBatch;

		scriptCallBatch.scriptPaths = entity2D.scriptPaths;
		scriptCallBatch.entityID = entity2D.ID;
		scriptCallBatch.sceneTreeID = this->ID;

		scriptCallBatches.push_back(scriptCallBatch);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneTree2D::generateOnFinalCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->entities2D.size(); i++)
	{
		Entity2D entity2D = this->entities2D.at(i);

		if (!this->finishEntitiesQueue.empty())
		{
			if (entity2D.ID == this->finishEntitiesQueue.front())
			{
				ScriptCallBatch scriptCallBatch;

				scriptCallBatch.scriptPaths = entity2D.scriptPaths;
				scriptCallBatch.entityID = entity2D.ID;
				scriptCallBatch.sceneTreeID = this->ID;

				scriptCallBatches.push_back(scriptCallBatch);
				this->finishEntitiesQueue.pop();
			}
		}
	}

	return scriptCallBatches;
}

Lilliputian::ComponentID Lilliputian::SceneTree2D::getCurrentCameraIndex()
{
	return this->currentCamera;
}

Lilliputian::Vector<Lilliputian::ComponentVariant>& Lilliputian::SceneTree2D::getComponentVariants()
{
	return this->componentVariants;
}

Lilliputian::Transform2D& Lilliputian::SceneTree2D::getEntityTransform(EntityID entityID)
{
	Transform2D* transform2D = this->dummyTransform2D;

	for (int i = 0; i < this->transform2DIndexCache.size(); i++)
	{
		ComponentID transform2DID = this->transform2DIndexCache.at(i);

		if (this->componentVariants.at(transform2DID).entityID == entityID)
			transform2D = this->componentVariants.at(transform2DID).transform2D;
	}

	return *transform2D;
}

void Lilliputian::SceneTree2D::setID(SceneTreeID ID)
{
	this->ID = ID;
}