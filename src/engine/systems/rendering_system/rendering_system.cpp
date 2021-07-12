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
#include "built_in_shaders.hpp"
#include <SDL_image.h>
#include <os/os.hpp>

Lilliputian::RenderingSystem::RenderingSystem(Window& window)
{
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	this->renderingContext = new RenderingContext(window);
}

Lilliputian::RenderingSystem::~RenderingSystem()
{
	IMG_Quit();
	delete renderingContext;
}

void Lilliputian::RenderingSystem::process(Scene& scene)
{
	std::vector<ComponentVariant> componentVariants = scene.getComponentVariants();
	std::vector<size_t> renderOrderIndexCache = scene.getRenderOrderIndexCache();
	std::vector<size_t> uiViewportIndexCache = scene.getUIViewportIndexCache();
	std::vector<Image> outputImages;

	std::vector<std::string> vertexShaderSources;
	std::vector<std::string> fragmentShaderSources;

	Camera* camera;
	Image image;

	this->renderingContext->clearBuffers();

	for (int i = 0; i < uiViewportIndexCache.size(); i++)
	{
		ComponentVariant& uiViewportComponent = componentVariants.at(uiViewportIndexCache.at(i));
		UIViewport* uiViewport = uiViewportComponent.getUIViewport();

		if (uiViewport->getIsVisible())
		{
			Entity& cameraEntity = scene.getEntity(uiViewport->getCameraEntityID());
			ComponentVariant cameraComponent = scene.getComponent(cameraEntity.components.at(ComponentVariant::Type::CAMERA));
			camera = cameraComponent.getCamera();

			if (camera->getIsStreaming())
			{
				Transform cameraTransform = scene.getEntityTransform(cameraComponent.getEntityID());
				Rectangle cameraViewport_px = camera->getViewportDimensions();

				for (int i = 0; i < renderOrderIndexCache.size(); i++)
				{
					ComponentVariant renderableComponent = componentVariants.at(renderOrderIndexCache.at(i));
					Transform transform = scene.getEntityTransform(renderableComponent.getEntityID());

					if (cameraEntity.spatialDimension == Entity::SpatialDimension::_2D)
					{
						if (scene.getEntity(renderableComponent.getEntityID()).spatialDimension ==
							Entity::SpatialDimension::_2D)
						{
							image = renderableComponent.getImage();
							outputImages.push_back(image);
						}
					}
				}
			}

			static bool flag = false;

			if (!flag)
			{
				vertexShaderSources.clear();
				fragmentShaderSources.clear();
				vertexShaderSources.push_back(BuiltInShaders::Vertex::texture);
				fragmentShaderSources.push_back(BuiltInShaders::Fragment::texture);
				this->renderingContext->getShaderCompiler().deleteProgram();
				this->renderingContext->getShaderCompiler().compileShaders(&vertexShaderSources, &fragmentShaderSources);
				this->renderingContext->delete2DTextures();
				this->renderingContext->generate2DTextures(outputImages);
				flag = true;
			}

			this->renderingContext->drawArrays();
			outputImages.clear();
		}		
	}

	OS::getWindow().swap();
}