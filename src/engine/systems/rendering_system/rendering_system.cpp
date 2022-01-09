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
#include <application/scene/assets/shader.hpp>
#include <application/scene/assets/image.hpp>

Esi::RenderingSystem::RenderingSystem()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	this->context = std::unique_ptr<RenderingContext>(new RenderingContext());
	this->shaderCompiler = std::unique_ptr<ShaderCompiler>(new ShaderCompiler());
}

Esi::RenderingSystem::~RenderingSystem()
{
	this->deinitialize();
}

void Esi::RenderingSystem::initialize()
{
	std::vector<Shader> shaders;
	Shader builtInVertexShader;
	Shader builtInFragmentShader;

	builtInVertexShader.setSource(BuiltInShaders::Vertex::texture, Shader::Type::VERTEX);
	builtInFragmentShader.setSource(BuiltInShaders::Fragment::texture, Shader::Type::FRAGMENT);
	shaders.push_back(builtInVertexShader);
	shaders.push_back(builtInFragmentShader);

	this->context->initialize();
	this->compileShaders("built_in_shaders", shaders);
	this->isInitialized = true;
}

void Esi::RenderingSystem::process(Scene& scene)
{
	this->buildRenderables(scene);
	this->context->clearBuffers();
	this->context->submit(this->getRenderables());
	this->context->swapBuffers();
}

void Esi::RenderingSystem::deinitialize()
{
	if (this->isInitialized)
		IMG_Quit();

	this->isInitialized = false;
}

void Esi::RenderingSystem::buildRenderables(Scene& scene)
{
	std::vector<std::shared_ptr<Component>>& components = scene.getComponents();
	std::vector<size_t> renderOrderIndexCache = scene.getRenderOrderIndexCache();
	std::vector<size_t> uiViewportIndexCache;

	if (scene.getComponentIndexCaches().count(UIViewport::TYPE_STRING))
		uiViewportIndexCache = scene.getComponentIndexCaches().at(UIViewport::TYPE_STRING);

	for (int i = 0; i < uiViewportIndexCache.size(); i++)
	{
		std::shared_ptr<UIViewport> uiViewport = std::dynamic_pointer_cast<UIViewport>(components.at(uiViewportIndexCache.at(i)));

		if (uiViewport->getIsVisible())
		{
			Entity& cameraEntity = scene.getEntity(uiViewport->getCameraEntityID());
			Component& cameraComponent = scene.getComponent(cameraEntity.components.at(Camera::TYPE_STRING));
			Camera& camera = dynamic_cast<Camera&>(cameraComponent);

			if (camera.getIsStreaming())
			{
				Transform& cameraTransform = scene.getEntityTransform(cameraComponent.getEntityID());
				Rectangle cameraViewport_px = camera.getViewportDimensions();

				for (int i = 0; i < renderOrderIndexCache.size(); i++)
				{
					std::shared_ptr<RenderableComponent> renderableComponent = 
						std::dynamic_pointer_cast<RenderableComponent>(components.at(renderOrderIndexCache.at(i)));
					Transform& transform = scene.getEntityTransform(renderableComponent->getEntityID());
					std::vector<Shader> shaders = scene.getEntity(renderableComponent->getEntityID()).shaders;

					if (cameraEntity.spatialDimension == Entity::SpatialDimension::_2D)
					{
						if (scene.getEntity(renderableComponent->getEntityID()).spatialDimension ==
							Entity::SpatialDimension::_2D)
						{
							Image& image = renderableComponent->getImage();
						}
					}
				}
			}
		}
	}
}

Esi::Renderables& Esi::RenderingSystem::getRenderables()
{
	return *this->renderables;
}

std::string Esi::RenderingSystem::getRenderingContextName()
{
	return this->context->getRenderingContextName();
}

void Esi::RenderingSystem::compileShaders(std::string name, std::vector<Shader> shaders)
{
	this->shaderProgramCache.emplace(name, this->shaderCompiler->compile(shaders));
}