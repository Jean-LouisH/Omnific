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
#include <application/scene/assets/shader.hpp>
#include <application/scene/assets/image.hpp>

Omnific::RenderingSystem::RenderingSystem()
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
}

Omnific::RenderingSystem::~RenderingSystem()
{
	this->deinitialize();
}

void Omnific::RenderingSystem::initialize()
{
	std::vector<Shader> shaders;
	Shader builtInVertexShader;
	Shader builtInFragmentShader;

	builtInVertexShader.setSource(BuiltInShaders::Vertex::texture, Shader::ShaderType::VERTEX);
	builtInFragmentShader.setSource(BuiltInShaders::Fragment::texture, Shader::ShaderType::FRAGMENT);
	shaders.push_back(builtInVertexShader);
	shaders.push_back(builtInFragmentShader);

	this->context->initialize();
	this->compileShaders(this->builtInShaderProgramName, shaders);
	this->isInitialized = true;
}

void Omnific::RenderingSystem::process(Scene& scene)
{
	this->onWindowResize();
	this->onModifiedRenderableInstance(scene);
	this->context->clearColourBuffer();
	this->context->submit(this->sceneRenderables);
	this->context->swapBuffers();
}

void Omnific::RenderingSystem::deinitialize()
{
	if (this->isInitialized)
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	this->isInitialized = false;
}

void Omnific::RenderingSystem::onWindowResize()
{
	Rectangle windowRectangle = OS::getWindow().getWindowSize();
	this->context->setViewport(windowRectangle.width, windowRectangle.height);
}

void Omnific::RenderingSystem::onModifiedShaderInstance(Scene& scene)
{
	if (scene.getHasShadersChanged())
	{

	}
}

void Omnific::RenderingSystem::onModifiedRenderableInstance(Scene& scene)
{
	if (scene.getHasRenderableComponentsChanged())
	{
		this->sceneRenderables.clear();

		std::vector<std::shared_ptr<UIViewport>> uiViewports = scene.getComponentsByType<UIViewport>();
		std::vector<size_t> renderOrderIndexCache = scene.getRenderOrderIndexCache();

		for (int i = 0; i < uiViewports.size(); i++)
		{
			std::shared_ptr<UIViewport> uiViewport = uiViewports.at(i);
			Entity& cameraEntity = scene.getEntity(uiViewport->getCameraEntityID());
			std::shared_ptr<Component> cameraComponent = scene.getComponent(cameraEntity.componentIDs.at(Camera::TYPE_STRING));
			std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(cameraComponent);
			SceneRenderable sceneRenderable;
			std::shared_ptr<Transform> cameraTransform = scene.getEntityTransform(cameraComponent->getEntityID());

			sceneRenderable.camera = camera;
			sceneRenderable.cameraTransform = cameraTransform;

			for (int i = 0; i < renderOrderIndexCache.size(); i++)
			{
				EntityRenderable renderable;
				std::shared_ptr<RenderableComponent> renderableComponent =
					std::dynamic_pointer_cast<RenderableComponent>(scene.getComponents().at(renderOrderIndexCache.at(i)));
				Entity& entity = scene.getEntity(renderableComponent->getEntityID());

				renderable.entityTransform = scene.getEntityTransform(renderableComponent->getEntityID());
				renderable.id = renderableComponent->getEntityID();
				renderable.shaderPrograms.push_back(this->shaderProgramCache.at(this->builtInShaderProgramName));

				renderable.vertexArray = std::shared_ptr<VertexArray>(new VertexArray());

				if (renderableComponent->isType(ModelContainer::TYPE_STRING))
				{
					std::shared_ptr<ModelContainer> modelContainer =
						std::dynamic_pointer_cast<ModelContainer>(renderableComponent);
					std::shared_ptr<Model> model = modelContainer->getCurrentModel();
					renderable.texture = std::shared_ptr<Texture>(new Texture(model->image));
					renderable.vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(model->mesh, renderable.vertexArray));
				}
				else
				{
					if (renderableComponent->isType(SpriteContainer::TYPE_STRING))
					{
						std::shared_ptr<SpriteContainer> spriteContainer =
							std::dynamic_pointer_cast<SpriteContainer>(renderableComponent);

						if (spriteContainer->isXBillboarded)
							renderable.entityTransform->rotation.x = cameraTransform->rotation.x;
						if (spriteContainer->isYBillboarded)
							renderable.entityTransform->rotation.y = cameraTransform->rotation.y;
					}
					std::shared_ptr<Image> image = renderableComponent->getImage();
					renderable.texture = std::shared_ptr<Texture>(new Texture(image));
					renderable.vertexBuffer = std::shared_ptr<VertexBuffer>(
						new VertexBuffer(image, renderableComponent->getDimensions(), renderable.vertexArray));
				}

				//if (cameraEntity.spatialDimension == Entity::SpatialDimension::_2D)
				//{
				//	if (scene.getEntity(renderableComponent->getEntityID()).spatialDimension ==
				//		Entity::SpatialDimension::_2D)
				//	{

				//	}
				//}

				sceneRenderable.entityRenderables.push_back(renderable);
			}

			this->sceneRenderables.push_back(sceneRenderable);
		}
	}
}

std::string Omnific::RenderingSystem::getRenderingContextName()
{
	return this->context->getRenderingContextName();
}

void Omnific::RenderingSystem::compileShaders(std::string name, std::vector<Shader> shaders)
{
	std::shared_ptr<ShaderProgram> shaderProgram = this->shaderCompiler->compile(shaders);

	if (shaderProgram)
		this->shaderProgramCache.emplace(name, shaderProgram);
}