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

#include <os/window.hpp>
#include <glad/glad.h>
#include <vector>
#include "shader_program.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include <application/scene/assets/image.hpp>
#include <application/scene/assets/shader.hpp>
#include <application/scene/scene.hpp>
#include "../scene_tree_renderable.hpp"
#include "utilities/aliases.hpp"
#include <string>
#include <map>

namespace Omnia
{
	/* The object that calls the rendering API functions. */
	class RenderingContext
	{
	public:
		void initialize();
		void clearColourBuffer();
		void clearDepthBuffer();
		void enableDepthTest();
		void disableDepthTest();
		void enableWireframeMode();
		void disableWireframeMode();
		void setViewport(uint32_t width, uint32_t height);
		void submit(std::map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists);
		void swapBuffers();
		std::string getRenderingContextName();
	private:
		SDL_GLContext glContext;

		std::unordered_map<AssetID, std::shared_ptr<Texture>> textures;
		std::unordered_map<AssetID, std::shared_ptr<VertexArray>> vertexArrays;
		std::unordered_map<AssetID, std::shared_ptr<ShaderProgram>> shaderPrograms;
		std::shared_ptr<ShaderProgram> builtInShaderProgram;

		uint8_t allowableMissedFrames = 0;
		std::unordered_map<AssetID, uint8_t> missedFrameCounts;

		std::shared_ptr<Texture> getTexture(std::shared_ptr<RenderableComponent> renderableComponent);
		std::shared_ptr<VertexArray> getVertexArray(std::shared_ptr<RenderableComponent> renderableComponent);

		void collectGarbage();
	};
}

