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

#include "rendering_context.hpp"
#include "built_in_shaders.hpp"
#include <os/os.hpp>
#include <string>
#include <SDL_video.h>

void Omnia::RenderingContext::initialize()
{
	Window& window = OS::getWindow();
	this->glContext = SDL_GL_CreateContext(window.getSDLWindow());

	if (!gladLoadGL())
	{
		OS::getLogger().write("GLAD failed to initialize.");
	}
	else
	{
		std::vector<Shader> shaders;
		Shader builtInVertexShader;
		Shader builtInFragmentShader;

		builtInVertexShader.setSource(BuiltInShaders::Vertex::texture, Shader::ShaderType::VERTEX);
		builtInFragmentShader.setSource(BuiltInShaders::Fragment::texture, Shader::ShaderType::FRAGMENT);
		shaders.push_back(builtInVertexShader);
		shaders.push_back(builtInFragmentShader);

		this->builtInShaderProgram = std::shared_ptr<ShaderProgram>(new ShaderProgram(shaders));

		Rectangle windowDimensions = window.getWindowSize();
		this->setViewport(windowDimensions.width, windowDimensions.height);
		OS::getLogger().write((std::string)("Rendering System initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
}

void Omnia::RenderingContext::clearColourBuffer()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnia::RenderingContext::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}


void Omnia::RenderingContext::submit(std::unordered_map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists)
{

	for (auto it = sceneTreeRenderableLists.begin(); it != sceneTreeRenderableLists.end(); it++)
	{

		std::vector<SceneTreeRenderable> sceneTreeRenderableList = it->second;
		SceneTreeRenderable* sceneTreeRenderableListData = sceneTreeRenderableList.data();
		size_t sceneTreeRenderableListCount = sceneTreeRenderableList.size();

		for (size_t i = 0; i < sceneTreeRenderableListCount; i++)
		{

			SceneTreeRenderable& sceneTreeRenderable = sceneTreeRenderableListData[i];

			if (sceneTreeRenderable.camera->getIsStreaming())
			{
				EntityRenderable* entityRenderablesData = sceneTreeRenderable.entityRenderables.data();
				size_t entityRenderablesCount = sceneTreeRenderable.entityRenderables.size();
				glm::mat4 worldToViewMatrix = glm::inverse(sceneTreeRenderable.cameraTransform->getGlobalTransformMatrix());
				glm::mat4 viewToProjectionMatrix = sceneTreeRenderable.camera->getViewToProjectionMatrix();

				this->clearDepthBuffer();

				for (size_t j = 0; j < entityRenderablesCount; j++)
				{

					EntityRenderable& entityRenderable = entityRenderablesData[j];
					std::vector<std::shared_ptr<Shader>> shaders = entityRenderable.renderableComponent->getShaders();
					size_t shaderCount = shaders.size();

					glm::mat4 modelToWorldMatrix = entityRenderable.entityTransform->getGlobalTransformMatrix();
					glm::mat4 mvp = viewToProjectionMatrix * worldToViewMatrix * modelToWorldMatrix;
					float alpha = entityRenderable.renderableComponent->getAlphaInPercentage();

					std::shared_ptr<VertexArray> vertexArray = this->getVertexArray(entityRenderable.renderableComponent);
					std::shared_ptr<Texture> texture = this->getTexture(entityRenderable.renderableComponent);
					vertexArray->bind();
					texture->bind();

					/* Temporary debug rotation*/
					if (entityRenderable.renderableComponent->isType(ModelContainer::TYPE_STRING))
						entityRenderable.entityTransform->rotation.y += 1;

					/* Render for each ShaderProgram. Starting with the built in one. */
					for (int k = -1; k < (int)shaderCount; k++)
					{
						std::shared_ptr<ShaderProgram> shaderProgram;

						if (k == -1)
							shaderProgram = this->builtInShaderProgram;
						else
							shaderProgram = this->shaderPrograms.at(shaders.at(k)->getID());

						shaderProgram->use();
						shaderProgram->setInt("textureSampler", 0);
						shaderProgram->setMat4("mvp", mvp);
						shaderProgram->setFloat("alpha", alpha);
						glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0);
					}

					vertexArray->unbind();
					texture->activateDefaultTextureUnit();
				}
			}
		}
	}

	this->collectGarbage();
}

void Omnia::RenderingContext::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnia::RenderingContext::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Omnia::RenderingContext::getRenderingContextName()
{
	return "opengl";
}

std::shared_ptr<Omnia::Texture> Omnia::RenderingContext::getTexture(std::shared_ptr<RenderableComponent> renderableComponent)
{
	std::shared_ptr<Image> image;

	if (renderableComponent->isType(ModelContainer::TYPE_STRING))
		image = std::dynamic_pointer_cast<ModelContainer>(renderableComponent)->getCurrentModel()->image;
	else
		image = renderableComponent->getImage();

	AssetID assetID = image->getID();

	if (this->textures.count(assetID) == 0)
	{
		this->textures.emplace(assetID, std::shared_ptr<Texture>(new Texture(image)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}
	return this->textures.at(assetID);
}

std::shared_ptr<Omnia::VertexArray> Omnia::RenderingContext::getVertexArray(std::shared_ptr<RenderableComponent> renderableComponent)
{
	AssetID assetID;

	if (renderableComponent->isType(ModelContainer::TYPE_STRING))
	{
		std::shared_ptr<Model> model = std::dynamic_pointer_cast<ModelContainer>(renderableComponent)->getCurrentModel();
		if (model != nullptr)
		{
			assetID = model->mesh->getID();
		}
	}
	else
	{
		std::shared_ptr<Image> image = renderableComponent->getImage();
		if (image != nullptr)
		{
			assetID = image->getID();
		}
	}

	if (this->vertexArrays.count(assetID) == 0)
	{
		this->vertexArrays.emplace(assetID, std::shared_ptr<VertexArray>(new VertexArray(renderableComponent)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}
	return this->vertexArrays.at(assetID);
}

void Omnia::RenderingContext::collectGarbage()
{
	for (auto it = this->missedFrameCounts.cbegin(); it != this->missedFrameCounts.cend(); it++)
	{
		if (this->missedFrameCounts.at(it->first) > this->allowableMissedFrames)
		{
			if (this->textures.count(it->first) > 0)
				this->textures.erase(it->first);
			if (this->vertexArrays.count(it->first) > 0)
				this->vertexArrays.erase(it->first);

			this->missedFrameCounts.erase(it);
		}
	}

	for (auto it = this->missedFrameCounts.cbegin(); it != this->missedFrameCounts.cend(); it++)
	{
		this->missedFrameCounts.at(it->first)++;
	}
}