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

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}
}

void Omnia::RenderingContext::clearColourBuffer()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
					AssetID vertexArrayAssetID = 0;
					AssetID textureAssetID = 0;

					if (entityRenderable.renderableComponent->isType(ModelContainer::TYPE_STRING))
					{
						std::shared_ptr<ModelContainer> modelContainer =
							std::dynamic_pointer_cast<ModelContainer>(entityRenderable.renderableComponent);
						std::shared_ptr<Model> model = modelContainer->getCurrentModel();
						vertexArrayAssetID = model->mesh->getID();
						textureAssetID = model->image->getID();
						this->getVertexArray(model->mesh)->bind();
						this->getTexture(model->image)->bind();
					}
					else
					{
						if (entityRenderable.renderableComponent->isType(SpriteContainer::TYPE_STRING))
						{
							std::shared_ptr<SpriteContainer> spriteContainer =
								std::dynamic_pointer_cast<SpriteContainer>(entityRenderable.renderableComponent);

							if (spriteContainer->isXBillboarded)
								entityRenderable.entityTransform->rotation.x = sceneTreeRenderable.cameraTransform->rotation.x;
							if (spriteContainer->isYBillboarded)
								entityRenderable.entityTransform->rotation.y = sceneTreeRenderable.cameraTransform->rotation.y;
						}

						std::shared_ptr<Image> image = entityRenderable.renderableComponent->getImage();
						vertexArrayAssetID = image->getID();
						textureAssetID = image->getID();
						this->getTexture(image)->bind();
						this->getVertexArray(image, entityRenderable.renderableComponent->getDimensions())->bind();
					}

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
						glDrawElements(GL_TRIANGLES, (GLsizei)this->vertexArrays.at(vertexArrayAssetID)->getIndexCount(), GL_UNSIGNED_INT, 0);
					}

					this->vertexArrays.at(vertexArrayAssetID)->unbind();
					if (this->textures.count(textureAssetID) > 0)
						this->textures.at(textureAssetID)->activateDefaultTextureUnit();

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

std::shared_ptr<Omnia::Texture> Omnia::RenderingContext::getTexture(std::shared_ptr<Image> image)
{
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

std::shared_ptr<Omnia::VertexArray> Omnia::RenderingContext::getVertexArray(std::shared_ptr<Mesh> mesh)
{
	AssetID assetID = mesh->getID();
	if (this->vertexArrays.count(assetID) == 0)
	{
		this->vertexArrays.emplace(assetID, std::shared_ptr<VertexArray>(new VertexArray(mesh)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}
	return this->vertexArrays.at(assetID);
}

std::shared_ptr<Omnia::VertexArray> Omnia::RenderingContext::getVertexArray(std::shared_ptr<Image> image, glm::vec3 dimensions)
{
	AssetID assetID = image->getID();
	if (this->vertexArrays.count(assetID) == 0)
	{
		this->vertexArrays.emplace(assetID, std::shared_ptr<VertexArray>(new VertexArray(image, dimensions)));
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