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

#include <SDL.h>
#include "scene/scene.hpp"
#include "system.hpp"
#include <set>
#include <vector>
#include "os/window.hpp"
#include "utilities/aliases.hpp"
#include "rendering_context/rendering_context.hpp"
#include "entity_renderable.hpp"
#include "scene_tree_renderable.hpp"
#include <memory>
#include <map>

namespace Omnia
{
	/* Processes RenderableComponents for output to graphics display. */
	class RenderingSystem : public System
	{
	public:
		RenderingSystem()
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
			this->type = TYPE_STRING;
			this->threadType = ThreadType::OUTPUT;
		}
		~RenderingSystem();

		static constexpr const char* TYPE_STRING = "RenderingSystem";

		virtual void initialize() override;
		virtual void onOutput(std::shared_ptr<Scene> scene) override;
		virtual void deinitialize() override;
		std::string getRenderingContextName();
	private:
		std::unique_ptr<RenderingContext> context;
		std::map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists;

		void onWindowResize();
		void buildRenderablesOnModifiedComponents(std::shared_ptr<Scene> scene);
	};
}

