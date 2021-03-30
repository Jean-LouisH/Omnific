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

#include "scene_forest.hpp"
#include "game/virtual_machine/script_call_batch.hpp"

Lilliputian::SceneForest::SceneForest()
{

}

Lilliputian::Stack<Lilliputian::SceneTree2D>* Lilliputian::SceneForest::getLastSceneTree2DStack()
{
	return &(this->sceneTree2DStacks.at(this->sceneTree2DStacks.size() - 1));
}

void Lilliputian::SceneForest::incrementSceneTree2D()
{
	Entity2D emptyEntity2D;
	SceneTree2D sceneTree2D;
	Stack<SceneTree2D> sceneTree2DStack;

	sceneTree2D.setID(this->sceneTreeCount);
	this->sceneTreeCount++;
	sceneTree2DStack.push(sceneTree2D);
	this->sceneTree2DStacks.push_back(sceneTree2DStack);
	this->addEntity2D(emptyEntity2D);
}

void Lilliputian::SceneForest::addEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2DStack()->top().addEntity2D(entity2D);
}

void Lilliputian::SceneForest::addEmptyEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2DStack()->top().addEntity2D(entity2D);
}

void Lilliputian::SceneForest::addNameToLastEntity(String name)
{

}

void Lilliputian::SceneForest::addParentToLastEntityByName(String name)
{

}

void Lilliputian::SceneForest::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.ID = this->componentIDCount;
	this->componentIDCount++;
	this->getLastSceneTree2DStack()->top().addComponent(entityID, componentVariant);
}

void Lilliputian::SceneForest::addComponentToLastEntity(ComponentVariant componentVariant)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->addComponent(lastEntityID, componentVariant);
}

Lilliputian::EntityID Lilliputian::SceneForest::getPreviousEntityID()
{
	return this->entityIDCount - 1;
}

Lilliputian::AssetCache& Lilliputian::SceneForest::getAssetCache()
{
	return this->assetCache;
}

Lilliputian::Vector<Lilliputian::Stack<Lilliputian::SceneTree2D>>& Lilliputian::SceneForest::getSceneTree2DStacks()
{
	return this->sceneTree2DStacks;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnStartCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches = 
			this->sceneTree2DStacks.at(i).top().generateOnStartCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(), 
			newScriptCallBatches.begin(), 
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnInputCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2DStacks.at(i).top().generateOnInputCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnFrameCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2DStacks.at(i).top().generateOnFrameCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnComputeCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2DStacks.at(i).top().generateOnComputeCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnLateCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2DStacks.at(i).top().generateOnLateCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnFinalBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2DStacks.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2DStacks.at(i).top().generateOnFinalCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}