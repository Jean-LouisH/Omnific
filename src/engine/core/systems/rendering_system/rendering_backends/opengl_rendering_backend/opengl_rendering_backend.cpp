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

#include "opengl_rendering_backend.hpp"
#include "opengl_built_in_shaders.hpp"
#include <core/singletons/os/os.hpp>
#include <string>
#include <SDL_video.h>

#include <core/components/model.hpp>

void Omnia::OpenGLRenderingBackend::initialize()
{
	Window& window = OS::getWindow();
	window.initializeWindowContext("opengl");

	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		OS::getLogger().write("GLAD failed to initialize.");
	}
	else
	{
		this->builtInShaderProgram2D = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			this->getDefault2DVertexInput(),
			this->getDefault2DFragmentInput(),
			false,
			false))));

		this->builtInShaderProgram3D = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			this->getDefault3DVertexInput(),
			this->getDefault3DFragmentInput(),
			false,
			false))));

		glm::vec2 windowSize = window.getWindowSize();
		this->setViewport(windowSize.x, windowSize.y);
		OS::getLogger().write((std::string)("Rendering Backend initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));
	}
}

void Omnia::OpenGLRenderingBackend::clearColourBuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnia::OpenGLRenderingBackend::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnia::OpenGLRenderingBackend::enableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Omnia::OpenGLRenderingBackend::disableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Omnia::OpenGLRenderingBackend::enableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnia::OpenGLRenderingBackend::disableBlending()
{
	glDisable(GL_BLEND);
}

void Omnia::OpenGLRenderingBackend::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnia::OpenGLRenderingBackend::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Omnia::OpenGLRenderingBackend::getRenderingBackendName()
{
	return "opengl";
}

std::string Omnia::OpenGLRenderingBackend::getDefault2DVertexInput()
{
	return OpenGLBuiltInShaders::Vertex::dimension_2;
}

std::string Omnia::OpenGLRenderingBackend::getDefault2DFragmentInput()
{
	return OpenGLBuiltInShaders::Fragment::dimension_2;
}

std::string Omnia::OpenGLRenderingBackend::getDefault3DVertexInput()
{
	return OpenGLBuiltInShaders::Vertex::dimension_3;
}

std::string Omnia::OpenGLRenderingBackend::getDefault3DFragmentInput()
{
	return OpenGLBuiltInShaders::Fragment::dimension_3;
}

std::shared_ptr<Omnia::OpenGLTexture> Omnia::OpenGLRenderingBackend::getTexture(std::shared_ptr<Asset> asset)
{
	std::shared_ptr<OpenGLTexture> texture;
	std::shared_ptr<Image> image = std::dynamic_pointer_cast<Image>(asset);

	if (image != nullptr)
	{
		AssetID assetID = image->getID();

		if (this->textures.count(assetID) == 0)
		{
			this->textures.emplace(assetID, std::shared_ptr<OpenGLTexture>(new OpenGLTexture(image)));
			this->missedFrameCounts.emplace(assetID, 0);
		}
		else
		{
			this->missedFrameCounts.at(assetID) = 0;
		}

		texture = this->textures.at(assetID);
	}
	else
	{
		texture = std::shared_ptr<OpenGLTexture>(new OpenGLTexture());
	}

	return texture;
}

std::shared_ptr<Omnia::OpenGLVertexArray> Omnia::OpenGLRenderingBackend::getVertexArray(std::shared_ptr<Asset> asset)
{
	AssetID assetID;
	
	if (asset != nullptr)
		assetID = asset->getID();

	if (this->vertexArrays.count(assetID) == 0)
	{
		this->vertexArrays.emplace(assetID, std::shared_ptr<OpenGLVertexArray>(new OpenGLVertexArray(asset)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}

	return this->vertexArrays.at(assetID);
}

void Omnia::OpenGLRenderingBackend::collectGarbage()
{
	std::vector<AssetID> assetsToDelete;

	for (auto it = this->missedFrameCounts.begin(); it != this->missedFrameCounts.end(); it++)
	{
		if (this->missedFrameCounts.at(it->first) > this->allowableMissedFrames)
		{
			if (this->textures.count(it->first) > 0)
				this->textures.erase(it->first);
			if (this->vertexArrays.count(it->first) > 0)
				this->vertexArrays.erase(it->first);

			assetsToDelete.push_back(it->first);
		}
	}

	for (size_t i = 0; i < assetsToDelete.size(); i++)
		this->missedFrameCounts.erase(assetsToDelete.at(i));

	for (auto it = this->missedFrameCounts.begin(); it != this->missedFrameCounts.end(); it++)
		this->missedFrameCounts.at(it->first)++;
}