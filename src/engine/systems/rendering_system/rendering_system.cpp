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
#include <application/scene/assets/shader.hpp>
#include <application/scene/assets/image.hpp>

Omnia::RenderingSystem::RenderingSystem()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->context = std::unique_ptr<RenderingContext>(new RenderingContext());
}

Omnia::RenderingSystem::~RenderingSystem()
{
	this->deinitialize();
}

void Omnia::RenderingSystem::initialize()
{
	this->context->initialize();
	this->isInitialized = true;
}

void Omnia::RenderingSystem::process(Scene& scene)
{
	this->onWindowResize();
	this->buildRenderablesOnModifiedComponents(scene);
	this->context->clearColourBuffer();
	this->context->submit(this->sceneTreeRenderableLists);
	this->context->swapBuffers();
}

void Omnia::RenderingSystem::deinitialize()
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

void Omnia::RenderingSystem::buildRenderablesOnModifiedComponents(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		SceneTree& sceneTree = it->second;

		if (sceneTree.hasRenderableComponentsChanged)
		{
			std::vector<SceneTreeRenderable> sceneTreeRenderableList;

			std::vector<std::shared_ptr<UIViewport>> uiViewports = sceneTree.getComponentsByType<UIViewport>();
			std::vector<size_t> renderOrderIndexCache = sceneTree.getRenderOrderIndexCache();

			for (int i = 0; i < uiViewports.size(); i++)
			{
				std::shared_ptr<UIViewport> uiViewport = uiViewports.at(i);
				Entity& cameraEntity = sceneTree.getEntity(uiViewport->getCameraEntityID());
				std::shared_ptr<Camera> camera = sceneTree.getComponent<Camera>(cameraEntity.id);
				SceneTreeRenderable sceneTreeRenderable;
				std::shared_ptr<Transform> cameraTransform = sceneTree.getEntityTransform(camera->getEntityID());

				sceneTreeRenderable.is2D = sceneTree.is2D;
				sceneTreeRenderable.camera = camera;
				sceneTreeRenderable.cameraTransform = cameraTransform;

				for (int i = 0; i < renderOrderIndexCache.size(); i++)
				{
					EntityRenderable entityRenderable;
					std::shared_ptr<RenderableComponent> renderableComponent =
						std::dynamic_pointer_cast<RenderableComponent>(sceneTree.getComponents().at(renderOrderIndexCache.at(i)));
					Entity& entity = sceneTree.getEntity(renderableComponent->getEntityID());

					entityRenderable.entityTransform = sceneTree.getEntityTransform(renderableComponent->getEntityID());
					entityRenderable.renderableComponent = renderableComponent;
					sceneTreeRenderable.entityRenderables.push_back(entityRenderable);
				}
				sceneTreeRenderableList.push_back(sceneTreeRenderable);
			}

			this->sceneTreeRenderableLists.emplace(sceneTree.getID(), sceneTreeRenderableList);
			sceneTree.hasRenderableComponentsChanged = false;
		}

	}
}

std::string Omnia::RenderingSystem::getRenderingContextName()
{
	return this->context->getRenderingContextName();
}