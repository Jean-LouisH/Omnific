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
#include <core/assets/shader.hpp>
#include <core/assets/image.hpp>

#include <core/singletons/os/os.hpp>
#include <core/components/camera.hpp>
#include <core/components/viewport.hpp>
#include <core/components/transform.hpp>
#include <core/components/gui.hpp>

#include <core/singletons/configuration.hpp>

Omnia::RenderingSystem::~RenderingSystem()
{
	this->finalize();
}

void Omnia::RenderingSystem::initialize()
{
	Image image = Image(
		OS::getFileAccess().getDataDirectoryPath() + Configuration::getInstance()->metadata.iconFilepath);
	OS::getWindow().changeIcon(
		image.getData(),
		image.getWidth(),
		image.getHeight(),
		image.getDepth(),
		image.getPitch());

	this->openglBackend->initialize();
	this->isInitialized = true;
}

void Omnia::RenderingSystem::onLate(std::shared_ptr<Scene> scene)
{
	this->onWindowResize();
	if (this->hasSceneChanged(scene))
		this->buildRenderables(scene);
	this->openglBackend->clearColourBuffer(0, 0, 0, 255);
	this->openglBackend->submit(this->renderableLayerLists);
	this->openglBackend->swapBuffers();
}

void Omnia::RenderingSystem::finalize()
{
	if (this->isInitialized)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

	this->isInitialized = false;
}

void Omnia::RenderingSystem::onWindowResize()
{
	glm::vec2 windowSize = OS::getWindow().getWindowSize();

	if (lastDetectedWindowSize.x != windowSize.x ||
		lastDetectedWindowSize.y != windowSize.y)
	{
		int renderableLayerListSize = this->renderableLayerLists.size();

		for (int i = 0; renderableLayerListSize; i++)
		{
			std::vector<RenderableLayer> renderableLayers = this->renderableLayerLists.at(renderableLayerListSize - 1);
			int renderableLayerSize = renderableLayers.size();
			if (renderableLayerSize > 0)
			{
				RenderableLayer& renderableLayer = renderableLayers.at(renderableLayerSize - 1);

				if (renderableLayer.entityRenderables.size() > 0)
				{
					EntityRenderable& entityRenderable = renderableLayer.entityRenderables.at(0);
					if (entityRenderable.renderableComponent->isType(GUI::TYPE_STRING))
					{
						renderableLayer.camera->setViewportWidth(windowSize.x);
						renderableLayer.camera->setViewportHeight(windowSize.y);
					}
				}
			}
		}

		this->openglBackend->setViewport(windowSize.x, windowSize.y);
		lastDetectedWindowSize = windowSize;
	}
}

void Omnia::RenderingSystem::buildRenderables(std::shared_ptr<Scene> scene)
{
	this->renderableLayerLists.clear();

	std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& sceneLayers = scene->getSceneLayers();
	std::vector<std::shared_ptr<RenderableComponent>> guiRenderableComponents;
	std::vector<std::shared_ptr<SceneLayer>> guiSceneLayers;

	for (auto it = sceneLayers.begin(); it != sceneLayers.end(); it++)
	{
		std::shared_ptr<SceneLayer> sceneLayer = it->second;
		std::vector<RenderableLayer> renderableLayerList;

		std::vector<std::shared_ptr<Viewport>> uiViewports = sceneLayer->getComponentsByType<Viewport>();
		std::vector<size_t> renderOrderIndexCache = sceneLayer->getRenderOrderIndexCache();

		/* One or more Viewport RenderableLayers for each SceneLayer. */
		for (int i = 0; i < uiViewports.size(); i++)
		{
			std::shared_ptr<Viewport> uiViewport = uiViewports.at(i);
			std::shared_ptr<Entity> cameraEntity = sceneLayer->getEntityByName(uiViewport->getCameraEntityName());
			std::shared_ptr<Camera> camera = sceneLayer->getComponentByType<Camera>(cameraEntity->getID());
			RenderableLayer renderableLayer;
			std::shared_ptr<Transform> cameraTransform = sceneLayer->getComponentByType<Transform>(camera->getEntityID());

			renderableLayer.is2D = sceneLayer->is2D;
			renderableLayer.camera = camera;
			renderableLayer.cameraTransform = cameraTransform;

			for (std::shared_ptr<Light> light : sceneLayer->getComponentsByType<Light>())
			{
				renderableLayer.lights.push_back(light);
				renderableLayer.lightTransforms.push_back(sceneLayer->getComponentByType<Transform>(light->getEntityID()));
			}

			/* Entity RenderableLayer for each Viewport*/
			for (int i = 0; i < renderOrderIndexCache.size(); i++)
			{
				EntityRenderable entityRenderable;
				std::shared_ptr<RenderableComponent> renderableComponent =
					std::dynamic_pointer_cast<RenderableComponent>(sceneLayer->getComponents().at(renderOrderIndexCache.at(i)));

				/* GUI Components are deferred to a final RenderableLayer, 
				   while other RenderableComponents are in the usual order. */
				if (renderableComponent->isType(GUI::TYPE_STRING))
				{
					guiRenderableComponents.push_back(renderableComponent);
					guiSceneLayers.push_back(sceneLayer);
				}
				else
				{
					std::shared_ptr<Entity> entity = sceneLayer->getEntity(renderableComponent->getEntityID());

					entityRenderable.entityTransform = sceneLayer->getComponentByType<Transform>(entity->getID());
					entityRenderable.renderableComponent = renderableComponent;

					std::shared_ptr<Entity> topEntity = entity;
					EntityID parentEntityID = entity->parentID;

					while (parentEntityID != 0)
					{
						topEntity = sceneLayer->getEntity(parentEntityID);
						parentEntityID = topEntity->parentID;
					}

					if (topEntity->renderableComponentID != 0)
					{
						std::shared_ptr<RenderableComponent> renderableComponent =
							std::dynamic_pointer_cast<RenderableComponent>(sceneLayer->getComponentByID(topEntity->renderableComponentID));

						std::shared_ptr<Shader> overridingShader = renderableComponent->getOverridingShader();

						if (overridingShader != nullptr)
						{
							entityRenderable.overridingShader = overridingShader;
							entityRenderable.overridingShaderParameters = renderableComponent->shaderParameters;
						}
					}

					renderableLayer.entityRenderables.push_back(entityRenderable);
				}
			}
			renderableLayerList.push_back(renderableLayer);
		}

		this->renderableLayerLists.push_back(renderableLayerList);
	}

	/* Put the deferred GUIs on a final RenderableLayer to be rendered last. */
	RenderableLayer renderableLayer;
	std::vector<RenderableLayer> renderableLayerList;
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<Transform> cameraTransform(new Transform());
	glm::vec2 windowSize = OS::getWindow().getWindowSize();

	/* a virtual Camera for the GUI. */
	camera->setViewportWidth(windowSize.x);
	camera->setViewportHeight(windowSize.y);
	camera->setIsStreaming(true);
	camera->setWireframeMode(false);
	cameraTransform->translation.x = windowSize.x / 2.0;
	cameraTransform->translation.y = windowSize.y / 2.0;

	renderableLayer.is2D = true;
	renderableLayer.camera = camera;
	renderableLayer.cameraTransform = cameraTransform;

	for (int i = 0; i < guiRenderableComponents.size(); i++)
	{
		EntityRenderable entityRenderable;
		std::shared_ptr<RenderableComponent> guiRenderableComponent = guiRenderableComponents[i];
		std::shared_ptr<SceneLayer> guiSceneLayer = guiSceneLayers[i];
		std::shared_ptr<Entity> entity = guiSceneLayer->getEntity(guiRenderableComponent->getEntityID());

		entityRenderable.entityTransform = guiSceneLayer->getComponentByType<Transform>(entity->getID());
		entityRenderable.renderableComponent = guiRenderableComponent;
		renderableLayer.entityRenderables.push_back(entityRenderable);
	}

	/* There is only a single virtual Viewport, so one RenderableLayer. */
	renderableLayerList.push_back(renderableLayer);
	this->renderableLayerLists.push_back(renderableLayerList);
}

std::string Omnia::RenderingSystem::getRenderingBackendName()
{
	return this->openglBackend->getRenderingBackendName();
}