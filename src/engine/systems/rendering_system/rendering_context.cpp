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
#include <os/os.hpp>
#include <string>
#include <SDL_video.h>

void Omnific::RenderingContext::initialize()
{
	Window& window = OS::getWindow();
	this->glContext = SDL_GL_CreateContext(window.getSDLWindow());

	if (!gladLoadGL())
	{
		OS::getLogger().write("GLAD failed to initialize.");
	}
	else
	{
		Rectangle windowDimensions = window.getWindowSize();
		this->setViewport(windowDimensions.width, windowDimensions.height);
		OS::getLogger().write((std::string)("Rendering System initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);
	}
}

void Omnific::RenderingContext::clearColourBuffer()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnific::RenderingContext::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}


void Omnific::RenderingContext::submit(std::unordered_map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists)
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

					if (entityRenderable.shaderPrograms.size() > 0)
					{
						std::shared_ptr<ShaderProgram>* shaderProgramsData = entityRenderable.shaderPrograms.data();
						glm::mat4 modelToWorldMatrix = entityRenderable.entityTransform->getGlobalTransformMatrix();
						glm::mat4 mvp = viewToProjectionMatrix * worldToViewMatrix * modelToWorldMatrix;
						size_t shaderCount = entityRenderable.shaderPrograms.size();

						entityRenderable.vertexArray->bind();
						entityRenderable.texture->bind();

						/* Render for each ShaderProgram. */
						for (size_t k = 0; k < shaderCount; k++)
						{
							std::shared_ptr<ShaderProgram> shaderProgram = shaderProgramsData[k];
							shaderProgram->use();
							shaderProgram->setInt("textureSampler", 0);
							shaderProgram->setMat4("mvp", mvp);
							glDrawElements(GL_TRIANGLES, (GLsizei)entityRenderable.vertexBuffer->getIndexCount(), GL_UNSIGNED_INT, 0);
						}
					}

					entityRenderable.vertexArray->unbind();
					entityRenderable.texture->activateDefaultTextureUnit();
				}
			}
		}
	}

}

void Omnific::RenderingContext::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnific::RenderingContext::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Omnific::RenderingContext::getRenderingContextName()
{
	return "opengl";
}