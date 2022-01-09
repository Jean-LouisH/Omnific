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

#include "physics_system.hpp"
#include "application/scene/scene.hpp"
#include <utilities/constants.hpp>
#include <os/os.hpp>

Esi::PhysicsSystem::~PhysicsSystem()
{
	this->deinitialize();
}

void Esi::PhysicsSystem::setMsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	this->msPerComputeUpdate = msPerComputeUpdate;
}

void Esi::PhysicsSystem::initialize()
{
	this->isInitialized = true;
}

void Esi::PhysicsSystem::process(Scene& scene)
{
	this->updateTimers(scene, this->msPerComputeUpdate);
}

void Esi::PhysicsSystem::deinitialize()
{
	this->isInitialized = false;
}

void Esi::PhysicsSystem::updateTimers(Scene& scene, uint32_t msPerComputeUpdate)
{
	std::vector<std::shared_ptr<Component>>& components = scene.getComponents();
	int componentVariantCount = components.size();

	for (int j = 0; j < componentVariantCount; j++)
	{
		std::shared_ptr<Component> component = components.at(j);
		if (component->getType() == CountdownTimer::TYPE_STRING)
		{
			std::dynamic_pointer_cast<CountdownTimer>(component)->update(msPerComputeUpdate * (1.0 / MS_IN_S));
		}
	}
}