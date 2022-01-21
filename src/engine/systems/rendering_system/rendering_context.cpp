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

void Esi::RenderingContext::initialize()
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

void Esi::RenderingContext::clearBuffers()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Esi::RenderingContext::submit(
		std::vector<Renderable> renderables,
		std::shared_ptr<Camera> camera,
		std::shared_ptr<Transform> cameraTransform,
		std::vector<std::shared_ptr<Light>> lights)
{
	Renderable* renderablesData = renderables.data();
	size_t renderablesCount = renderables.size();
	glm::mat4 worldToViewMatrix = glm::inverse(cameraTransform->getGlobalTransformMatrix());
	glm::mat4 viewToProjectionMatrix = camera->getViewToProjectionMatrix();

	for (size_t i = 0; i < renderablesCount; ++i)
	{
		Renderable& renderable = renderablesData[i];

		if (renderable.shaderPrograms.size() > 0)
		{
			std::shared_ptr<ShaderProgram>* shaderProgramsData = renderable.shaderPrograms.data();
			glm::mat4 modelToWorldMatrix = renderable.entityTransform->getGlobalTransformMatrix();
			size_t shaderCount = renderable.shaderPrograms.size();

			renderable.vertexArray->bind();
			renderable.texture->bind();

			for (size_t j = 0; j < shaderCount; j++)
			{
				std::shared_ptr<ShaderProgram> shaderProgram = shaderProgramsData[j];
				shaderProgram->use();
				shaderProgram->setInt("textureSampler", 0);
				shaderProgram->setMat4("modelToWorldMatrix", modelToWorldMatrix);
				shaderProgram->setMat4("worldToViewMatrix", worldToViewMatrix);
				shaderProgram->setMat4("viewToProjectionMatrix", viewToProjectionMatrix);
				glDrawElements(GL_TRIANGLES, (GLsizei)renderable.vertexBuffer->getIndexCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}

void Esi::RenderingContext::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Esi::RenderingContext::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Esi::RenderingContext::getRenderingContextName()
{
	return "opengl";
}