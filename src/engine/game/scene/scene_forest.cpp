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

Lilliputian::SceneTree2D* Lilliputian::SceneForest::getLastSceneTree2D()
{
	return &(this->sceneTree2Ds.at(this->sceneTree2Ds.size() - 1));
}

void Lilliputian::SceneForest::incrementSceneTree2D()
{
	Entity2D emptyEntity2D;
	SceneTree2D sceneTree2D;

	sceneTree2D.setID(this->sceneTreeCount);
	this->sceneTreeCount++;
	this->sceneTree2Ds.push_back(sceneTree2D);
	this->addEntity2D(emptyEntity2D);
}

void Lilliputian::SceneForest::addEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2D()->addEntity2D(entity2D);
}

void Lilliputian::SceneForest::addEmptyEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2D()->addEntity2D(entity2D);
}

void Lilliputian::SceneForest::addNameToLastEntity(String name)
{

}

void Lilliputian::SceneForest::addParentToLastEntityByName(String name)
{

}

void Lilliputian::SceneForest::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.setComponentID(this->componentIDCount);
	this->componentIDCount++;
	this->getLastSceneTree2D()->addComponent(entityID, componentVariant);
}

void Lilliputian::SceneForest::addComponentToLastEntity(ComponentVariant componentVariant)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->addComponent(lastEntityID, componentVariant);
}

void Lilliputian::SceneForest::addScript(EntityID entityID, String scriptPath)
{
	this->getLastSceneTree2D()->addScript(entityID, scriptPath);
}

void Lilliputian::SceneForest::addScriptToLastEntity(String scriptPath)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->addScript(lastEntityID, scriptPath);
}

Lilliputian::EntityID Lilliputian::SceneForest::getPreviousEntityID()
{
	return this->entityIDCount - 1;
}

Lilliputian::AssetCache& Lilliputian::SceneForest::getAssetCache()
{
	return this->assetCache;
}

Lilliputian::Vector<Lilliputian::SceneTree2D>& Lilliputian::SceneForest::getSceneTree2Ds()
{
	return this->sceneTree2Ds;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::SceneForest::generateOnStartCallBatches()
{
	Vector<ScriptCallBatch> scriptCallBatches;

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches = 
			this->sceneTree2Ds.at(i).generateOnStartCallBatches();

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

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2Ds.at(i).generateOnInputCallBatches();

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

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2Ds.at(i).generateOnFrameCallBatches();

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

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2Ds.at(i).generateOnComputeCallBatches();

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

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2Ds.at(i).generateOnLateCallBatches();

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

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
	{
		Vector<ScriptCallBatch> newScriptCallBatches =
			this->sceneTree2Ds.at(i).generateOnFinalCallBatches();

		scriptCallBatches.insert(
			scriptCallBatches.end(),
			newScriptCallBatches.begin(),
			newScriptCallBatches.end()
		);
	}

	return scriptCallBatches;
}