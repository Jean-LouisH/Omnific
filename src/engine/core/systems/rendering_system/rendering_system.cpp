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
	this->openglBackend->submit(this->sceneLayerRenderableLists);
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
	this->openglBackend->setViewport(windowSize.x, windowSize.y);
}

void Omnia::RenderingSystem::buildRenderables(std::shared_ptr<Scene> scene)
{
	this->sceneLayerRenderableLists.clear();

	std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& sceneLayers = scene->getSceneLayers();

	for (auto it = sceneLayers.begin(); it != sceneLayers.end(); it++)
	{
		std::shared_ptr<SceneLayer> sceneLayer = it->second;
		std::vector<SceneLayerRenderable> sceneLayerRenderableList;

		std::vector<std::shared_ptr<Viewport>> uiViewports = sceneLayer->getComponentsByType<Viewport>();
		std::vector<size_t> renderOrderIndexCache = sceneLayer->getRenderOrderIndexCache();

		for (int i = 0; i < uiViewports.size(); i++)
		{
			std::shared_ptr<Viewport> uiViewport = uiViewports.at(i);
			std::shared_ptr<Entity> cameraEntity = sceneLayer->getEntityByName(uiViewport->getCameraEntityName());
			std::shared_ptr<Camera> camera = sceneLayer->getComponentByType<Camera>(cameraEntity->getID());
			SceneLayerRenderable sceneLayerRenderable;
			std::shared_ptr<Transform> cameraTransform = sceneLayer->getComponentByType<Transform>(camera->getEntityID());

			sceneLayerRenderable.is2D = sceneLayer->is2D;
			sceneLayerRenderable.camera = camera;
			sceneLayerRenderable.cameraTransform = cameraTransform;

			for (std::shared_ptr<Light> light : sceneLayer->getComponentsByType<Light>())
			{
				sceneLayerRenderable.lights.push_back(light);
				sceneLayerRenderable.lightTransforms.push_back(sceneLayer->getComponentByType<Transform>(light->getEntityID()));
			}

			for (int i = 0; i < renderOrderIndexCache.size(); i++)
			{
				EntityRenderable entityRenderable;
				std::shared_ptr<RenderableComponent> renderableComponent =
					std::dynamic_pointer_cast<RenderableComponent>(sceneLayer->getComponents().at(renderOrderIndexCache.at(i)));
				std::shared_ptr<Entity> entity = sceneLayer->getEntity(renderableComponent->getEntityID());

				entityRenderable.entityTransform = sceneLayer->getComponentByType<Transform>(entity->getID());
				entityRenderable.renderableComponent = renderableComponent;

				std::shared_ptr<Entity> topEntity = entity;
				EntityID parentEntityID = entity->parentID;

				while(parentEntityID != 0)
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

				sceneLayerRenderable.entityRenderables.push_back(entityRenderable);
			}
			sceneLayerRenderableList.push_back(sceneLayerRenderable);
		}

		this->sceneLayerRenderableLists.emplace(sceneLayer->getID(), sceneLayerRenderableList);
	}
}

std::string Omnia::RenderingSystem::getRenderingBackendName()
{
	return this->openglBackend->getRenderingBackendName();
}