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

void Lilliputian::Scene::addSceneTree2D(SceneTree2D sceneTree2D)
{
	this->sceneTree2Ds.push_back(sceneTree2D);
}

void Lilliputian::Scene::removeSceneTree2D(SceneTreeID sceneTreeID)
{
	for (auto it = this->sceneTree2Ds.begin(); it != this->sceneTree2Ds.end();)
		if (it->getID() == sceneTreeID)
			it = this->sceneTree2Ds.erase(it);
		else
			++it;
}

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnStartCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnInputCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnFrameCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnComputeCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnLateCallBatches()
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

Lilliputian::Vector<Lilliputian::ScriptCallBatch> Lilliputian::Scene::getAllOnFinalBatches()
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

Lilliputian::SceneTree2D& Lilliputian::Scene::getLastSceneTree2D()
{
	return this->sceneTree2Ds.at(this->sceneTree2Ds.size() - 1);
}

Lilliputian::AssetCache& Lilliputian::Scene::getAssetCache()
{
	return this->assetCache;
}

Lilliputian::SceneTree2D& Lilliputian::Scene::getSceneTree(SceneTreeID sceneTreeID)
{
	SceneTree2D* sceneTree2D = nullptr;

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
		if (this->sceneTree2Ds.at(i).getID() == sceneTreeID)
			sceneTree2D = &this->sceneTree2Ds.at(i);

	return *sceneTree2D;
}

Lilliputian::Vector<Lilliputian::SceneTree2D>& Lilliputian::Scene::getSceneTree2Ds()
{
	return this->sceneTree2Ds;
}

void Lilliputian::Scene::unload()
{
	this->assetCache.deleteAll();

	for (int i = 0; i < this->sceneTree2Ds.size(); i++)
		this->sceneTree2Ds.at(i).unload();

	this->sceneTree2Ds.clear();
}