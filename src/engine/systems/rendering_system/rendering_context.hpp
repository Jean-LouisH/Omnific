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
#include "shader_compiler.hpp"
#include <vector>
#include "entity_renderable.hpp"
#include <application/scene/assets/image.hpp>
#include <application/scene/scene.hpp>
#include "scene_tree_renderable.hpp"
#include <string>

namespace Omnific
{
	/* The object that calls the rendering API functions. */
	class RenderingContext
	{
	public:
		void initialize();
		void clearColourBuffer();
		void clearDepthBuffer();
		void setViewport(uint32_t width, uint32_t height);
		void submit(std::unordered_map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists);
		void swapBuffers();
		std::string getRenderingContextName();
	private:
		SDL_GLContext glContext;
		std::vector<GLuint> glTextureIDs;
		std::vector<std::string> glslVertexShaderSources;
		std::vector<std::string> glslFragmentShaderSources;
	};
}

