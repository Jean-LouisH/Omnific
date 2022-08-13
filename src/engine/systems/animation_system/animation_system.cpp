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

#include "animation_system.hpp"
#include "scene/scene.hpp"

Omnia::AnimationSystem::~AnimationSystem()
{
	this->deinitialize();
}

void Omnia::AnimationSystem::setMsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	this->msPerComputeUpdate = msPerComputeUpdate;
}

void Omnia::AnimationSystem::initialize()
{
	this->isInitialized = true;
}

void Omnia::AnimationSystem::onCompute(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		this->updateSpriteContainers(it->second);
	}
}

void Omnia::AnimationSystem::deinitialize()
{
	this->isInitialized = false;
}

void Omnia::AnimationSystem::updateSpriteContainers(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<SpriteContainer>> spriteContainers = sceneTree->getComponentsByType<SpriteContainer>();

	for (size_t i = 0; i < spriteContainers.size(); i++)
	{
		spriteContainers.at(i)->update(this->msPerComputeUpdate * 1.0 / MS_IN_S);
	}
}