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
#include "component.hpp"
#include "application/scripting/python_vm/script_call_batch.hpp"

Omnific::Scene::Scene()
{
	this->id = UIDGenerator::getNewUID();
}

void Omnific::Scene::addSceneTree(SceneTree sceneTree)
{
	this->sceneTrees.emplace(sceneTree.getID(), sceneTree);
	this->lastSceneTreeID = sceneTree.getID();
}

void Omnific::Scene::addEmptySceneTree()
{
	SceneTree sceneTree;
	this->addSceneTree(sceneTree);
}

void Omnific::Scene::removeSceneTree(SceneTreeID sceneTreeID)
{
	if (this->sceneTrees.count(sceneTreeID))
		this->sceneTrees.erase(sceneTreeID);
}

Omnific::SceneTree& Omnific::Scene::getSceneTree(SceneTreeID sceneTree)
{
	return this->sceneTrees.at(sceneTree);
}

Omnific::SceneTree& Omnific::Scene::getSceneTreeByName(std::string name)
{
	SceneTree* sceneTree = nullptr;

	for (auto it = this->sceneTrees.begin(); it != this->sceneTrees.end(); it++)
		if (it->second.getName() == name)
			return it->second;

	return *sceneTree;
}

Omnific::SceneTree& Omnific::Scene::getLastSceneTree()
{
	return this->sceneTrees.at(this->lastSceneTreeID);
}

std::unordered_map<Omnific::SceneTreeID, Omnific::SceneTree>& Omnific::Scene::getSceneTrees()
{
	return this->sceneTrees;
}

Omnific::SceneID Omnific::Scene::getID()
{
	return this->id;
}