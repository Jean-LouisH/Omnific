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

#include "rendering_system.hpp"
#include <customization/classes/assets/shader.hpp>
#include <customization/classes/assets/image.hpp>

#include <singletons/os/os.hpp>
#include <customization/classes/components/camera.hpp>
#include <customization/classes/components/viewport.hpp>
#include <customization/classes/components/transform.hpp>

Omnia::RenderingSystem::~RenderingSystem()
{
	this->finalize();
}

void Omnia::RenderingSystem::initialize()
{
	this->context->initialize();
	this->isInitialized = true;
}

void Omnia::RenderingSystem::onLate(std::shared_ptr<Scene> scene)
{
	this->onWindowResize();
	this->buildRenderablesOnModifiedComponents(scene);
	this->context->clearColourBuffer(0, 0, 0, 255);
	this->context->submit(this->sceneTreeRenderableLists);
	this->context->swapBuffers();
}

void Omnia::RenderingSystem::finalize()
{
	if (this->isInitialized)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

	this->isInitialized = false;
}

void Omnia::RenderingSystem::onWindowResize()
{
	Rectangle windowRectangle = OS::getWindow().getWindowSize();
	this->context->setViewport(windowRectangle.width, windowRectangle.height);
}

void Omnia::RenderingSystem::buildRenderablesOnModifiedComponents(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		std::shared_ptr<SceneTree> sceneTree = it->second;

		if (sceneTree->hasRenderableComponentsChanged)
		{
			std::vector<SceneTreeRenderable> sceneTreeRenderableList;

			std::vector<std::shared_ptr<Viewport>> uiViewports = sceneTree->getComponentsByType<Viewport>();
			std::vector<size_t> renderOrderIndexCache = sceneTree->getRenderOrderIndexCache();

			for (int i = 0; i < uiViewports.size(); i++)
			{
				std::shared_ptr<Viewport> uiViewport = uiViewports.at(i);
				std::shared_ptr<Entity> cameraEntity = sceneTree->getEntityByName(uiViewport->getCameraEntityName());
				std::shared_ptr<Camera> camera = sceneTree->getComponentByType<Camera>(cameraEntity->getID());
				SceneTreeRenderable sceneTreeRenderable;
				std::shared_ptr<Transform> cameraTransform = sceneTree->getComponentByType<Transform>(camera->getEntityID());

				sceneTreeRenderable.is2D = sceneTree->is2D;
				sceneTreeRenderable.camera = camera;
				sceneTreeRenderable.cameraTransform = cameraTransform;

				for (int i = 0; i < renderOrderIndexCache.size(); i++)
				{
					EntityRenderable entityRenderable;
					std::shared_ptr<RenderableComponent> renderableComponent =
						std::dynamic_pointer_cast<RenderableComponent>(sceneTree->getComponents().at(renderOrderIndexCache.at(i)));
					std::shared_ptr<Entity> entity = sceneTree->getEntity(renderableComponent->getEntityID());

					entityRenderable.entityTransform = sceneTree->getComponentByType<Transform>(entity->getID());
					entityRenderable.renderableComponent = renderableComponent;
					sceneTreeRenderable.entityRenderables.push_back(entityRenderable);
				}
				sceneTreeRenderableList.push_back(sceneTreeRenderable);
			}

			this->sceneTreeRenderableLists.emplace(sceneTree->getID(), sceneTreeRenderableList);
			sceneTree->hasRenderableComponentsChanged = false;
		}

	}
}

std::string Omnia::RenderingSystem::getRenderingContextName()
{
	return this->context->getRenderingContextName();
}