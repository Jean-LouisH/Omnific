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
#include "game/scripting/virtual_machine/script_call_batch.hpp"

Lilliputian::Scene::Scene()
{

}

Lilliputian::SceneTree2D* Lilliputian::Scene::getLastSceneTree2D()
{
	return &(this->sceneTree2Ds.at(this->sceneTree2Ds.size() - 1));
}

void Lilliputian::Scene::incrementSceneTree2D()
{
	Entity2D emptyEntity2D;
	SceneTree2D sceneTree2D;

	sceneTree2D.setID(this->sceneTreeCount);
	this->sceneTreeCount++;
	this->sceneTree2Ds.push_back(sceneTree2D);
	this->addEntity2D(emptyEntity2D);
}

void Lilliputian::Scene::addEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2D()->addEntity2D(entity2D);
}

void Lilliputian::Scene::addEmptyEntity2D(Entity2D entity2D)
{
	entity2D.ID = this->entityIDCount;
	this->entityIDCount++;
	this->getLastSceneTree2D()->addEntity2D(entity2D);
}

void Lilliputian::Scene::addNameToLastEntity(String name)
{

}

void Lilliputian::Scene::addParentToLastEntityByName(String name)
{

}

void Lilliputian::Scene::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.setComponentID(this->componentIDCount);
	this->componentIDCount++;
	this->getLastSceneTree2D()->addComponent(entityID, componentVariant);
}

void Lilliputian::Scene::addComponentToLastEntity(ComponentVariant componentVariant)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->addComponent(lastEntityID, componentVariant);
}

void Lilliputian::Scene::addScript(EntityID entityID, String script)
{
	this->getLastSceneTree2D()->addScript(entityID, script);
}

void Lilliputian::Scene::addScriptToLastEntity(String script)
{
	EntityID lastEntityID = this->getPreviousEntityID();
	this->addScript(lastEntityID, script);
}

Lilliputian::EntityID Lilliputian::Scene::getPreviousEntityID()
{
	return this->entityIDCount - 1;
}

Lilliputian::AssetCache& Lilliputian::Scene::getAssetCache()
{
	return this->assetCache;
}

Lilliputian::Vector<Lilliputian::SceneTree2D>& Lilliputian::Scene::getSceneTree2Ds()
{
	return this->sceneTree2Ds;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnStartCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnInputCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnFrameCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnComputeCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnLateCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::generateOnFinalBatches()
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

void Lilliputian::Scene::unload()
{
	this->assetCache.deleteAll();

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
		this->sceneTree2Ds.at(i).unload();

	this->sceneTree2Ds.clear();
}