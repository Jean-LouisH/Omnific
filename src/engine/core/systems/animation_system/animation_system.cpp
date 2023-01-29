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
#include "core/scene/scene.hpp"
#include <core/singletons/configuration.hpp>
#include <core/singletons/os/os.hpp>

#include <core/components/sprite.hpp>

Omnia::AnimationSystem::~AnimationSystem()
{
	this->finalize();
}

void Omnia::AnimationSystem::initialize()
{
	this->isInitialized = true;
	OS::getLogger().write("Initialized Animation System");
}

void Omnia::AnimationSystem::onCompute(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		this->updateSpriteContainers(it->second);
	}
}

void Omnia::AnimationSystem::finalize()
{
	this->isInitialized = false;
}

void Omnia::AnimationSystem::updateSpriteContainers(std::shared_ptr<SceneTree> sceneTree)
{
	const uint32_t msPerComputeUpdate = Configuration::getInstance()->timeSettings.msPerComputeUpdate;
	std::vector<std::shared_ptr<Sprite>> spriteContainers = sceneTree->getComponentsByType<Sprite>();

	for (size_t i = 0; i < spriteContainers.size(); i++)
	{
		spriteContainers.at(i)->update(msPerComputeUpdate * 1.0 / MS_IN_S);
	}
}