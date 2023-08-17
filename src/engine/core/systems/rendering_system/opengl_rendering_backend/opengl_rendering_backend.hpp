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

#pragma once

#include <core/singletons/os/window.hpp>
#include <glad/glad.h>
#include <vector>
#include "opengl_shader_program.hpp"
#include "opengl_texture.hpp"
#include "opengl_vertex_array.hpp"
#include <core/assets/image.hpp>
#include <core/assets/shader.hpp>
#include <core/scene/scene.hpp>
#include "../renderable_layer.hpp"
#include "core/utilities/aliases.hpp"
#include <string>
#include <map>

namespace Omnia
{
	/* The object that calls the rendering API functions. */
	class OpenGLRenderingBackend
	{
	public:
		OpenGLRenderingBackend()
		{
			this->dummyLight = std::shared_ptr<Light>(new Light());
			this->dummyLightTransform = std::shared_ptr<Transform>(new Transform());
		}

		void initialize();
		void clearColourBuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		void clearDepthBuffer();
		void enableDepthTest();
		void disableDepthTest();
		void enableWireframeMode();
		void disableWireframeMode();
		void enableBlending();
		void disableBlending();
		void setViewport(uint32_t width, uint32_t height);
		void submit(std::vector<std::vector<RenderableLayer>> renderableLayerLists);
		void swapBuffers();
		std::string getRenderingBackendName();
	private:
		std::unordered_map<AssetID, std::shared_ptr<OpenGLTexture>> textures;
		std::unordered_map<AssetID, std::shared_ptr<OpenGLVertexArray>> vertexArrays;
		std::unordered_map<AssetID, std::shared_ptr<OpenGLShaderProgram>> shaderPrograms;
		std::shared_ptr<OpenGLShaderProgram> builtInShaderProgram2D;
		std::shared_ptr<OpenGLShaderProgram> builtInShaderProgram3D;

		std::shared_ptr<Light> dummyLight;
		std::shared_ptr<Transform> dummyLightTransform;

		uint8_t allowableMissedFrames = 0;
		std::unordered_map<AssetID, uint8_t> missedFrameCounts;

		std::shared_ptr<OpenGLTexture> getTexture(std::shared_ptr<Image> image);
		std::shared_ptr<OpenGLVertexArray> getVertexArray(std::shared_ptr<RenderableComponent> renderableComponent);

		void collectGarbage();
	};
}

