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
	this->shaderCompiler = std::unique_ptr<ShaderCompiler>(new ShaderCompiler());

	this->currentCamera = std::shared_ptr<Camera>(new Camera());
	this->currentCameraTransform = std::shared_ptr<Transform>(new Transform());
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

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	builtInVertexShader.setSource(BuiltInShaders::Vertex::texture, Shader::ShaderType::VERTEX);
	builtInFragmentShader.setSource(BuiltInShaders::Fragment::texture, Shader::ShaderType::FRAGMENT);
	shaders.push_back(builtInVertexShader);
	shaders.push_back(builtInFragmentShader);

	this->context->initialize();
	this->compileShaders(this->builtInShaderProgramName, shaders);
	this->isInitialized = true;
}

void Esi::RenderingSystem::process(Scene& scene)
{
	this->onWindowResize();
	this->onModifiedRenderableInstance(scene);
	this->context->clearBuffers();
	this->context->submit(
		this->getRenderables(),
		this->currentCamera, 
		this->currentCameraTransform, 
		this->lights);
	this->context->swapBuffers();
}

void Esi::RenderingSystem::deinitialize()
{
	if (this->isInitialized)
	{
		IMG_Quit();
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	this->isInitialized = false;
}

void Esi::RenderingSystem::onWindowResize()
{
	Rectangle windowRectangle = OS::getWindow().getWindowSize();
	this->context->setViewport(windowRectangle.width, windowRectangle.height);
}

void Esi::RenderingSystem::onModifiedShaderInstance(Scene& scene)
{
	if (scene.getHasShadersChanged())
	{

	}
}

void Esi::RenderingSystem::onModifiedRenderableInstance(Scene& scene)
{
	if (scene.getHasRenderableComponentsChanged())
	{
		this->renderables.clear();
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
				std::shared_ptr<Component> cameraComponent = scene.getComponent(cameraEntity.components.at(Camera::TYPE_STRING));
				std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(cameraComponent);

				if (camera->getIsStreaming())
				{
					std::shared_ptr<Transform> cameraTransform = scene.getEntityTransform(cameraComponent->getEntityID());
					Rectangle cameraViewport_px = camera->getViewportDimensions();

					this->currentCamera = camera;
					this->currentCameraTransform = cameraTransform;

					for (int i = 0; i < renderOrderIndexCache.size(); i++)
					{
						Renderable renderable;
						std::shared_ptr<RenderableComponent> renderableComponent =
							std::dynamic_pointer_cast<RenderableComponent>(components.at(renderOrderIndexCache.at(i)));
						Entity entity = scene.getEntity(renderableComponent->getEntityID());

						renderable.entityTransform = scene.getEntityTransform(renderableComponent->getEntityID());
						renderable.id = scene.getEntity(renderableComponent->getEntityID()).id;
						renderable.shaderPrograms.push_back(this->shaderProgramCache.at(this->builtInShaderProgramName));

						renderable.vertexArray = std::shared_ptr<VertexArray>(new VertexArray());

						if (renderableComponent->getType() == ModelContainer::TYPE_STRING)
						{
							std::shared_ptr<ModelContainer> modelContainer =
								std::dynamic_pointer_cast<ModelContainer>(renderableComponent);
							std::shared_ptr<Model> model = modelContainer->getCurrentModel();
							renderable.texture = std::shared_ptr<Texture>(new Texture(model->image));
							renderable.vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(model->mesh, renderable.vertexArray));
						}
						else
						{
							std::shared_ptr<Image> image = renderableComponent->getImage();
							renderable.texture = std::shared_ptr<Texture>(new Texture(image));
							renderable.vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(image, renderable.vertexArray));
						}

						//if (cameraEntity.spatialDimension == Entity::SpatialDimension::_2D)
						//{
						//	if (scene.getEntity(renderableComponent->getEntityID()).spatialDimension ==
						//		Entity::SpatialDimension::_2D)
						//	{

						//	}
						//}

						this->renderables.push_back(renderable);
					}
				}
			}
		}
	}
}

std::vector<Esi::Renderable> Esi::RenderingSystem::getRenderables()
{
	return this->renderables;
}

std::string Esi::RenderingSystem::getRenderingContextName()
{
	return this->context->getRenderingContextName();
}

void Esi::RenderingSystem::compileShaders(std::string name, std::vector<Shader> shaders)
{
	std::shared_ptr<ShaderProgram> shaderProgram = this->shaderCompiler->compile(shaders);

	if (shaderProgram)
		this->shaderProgramCache.emplace(name, shaderProgram);
}