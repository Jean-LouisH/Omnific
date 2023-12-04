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
#include "core/scene/scene.hpp"
#include "core/system.hpp"
#include <set>
#include <vector>
#include "core/singletons/os/window.hpp"
#include "core/utilities/aliases.hpp"
#include "rendering_backends/opengl_rendering_backend/opengl_rendering_backend.hpp"
#include "renderable.hpp"
#include "renderable_layer.hpp"
#include <memory>
#include <map>
#include <core/singletons/os/os.hpp>

namespace Omnia
{
	/* Processes Renderables for output to graphics display. */
	class RenderingSystem : public System
	{
	public:
		RenderingSystem();
		~RenderingSystem();

		static constexpr const char* TYPE_STRING = "RenderingSystem";

		virtual Registerable* instance() override
		{
			return new RenderingSystem(*this);
		}

		virtual void initialize() override;
		virtual void on_late(std::shared_ptr<Scene> scene) override;
		virtual void finalize() override;
		std::string get_rendering_backend_name();
	private:
		std::shared_ptr<OpenGLRenderingBackend> opengl_backend;
		std::vector<std::vector<RenderableLayer>> renderable_layer_lists;
		SceneID active_scene_id = 0;
		glm::vec2 last_detected_window_size;

		std::shared_ptr<Light> dummy_light;
		std::shared_ptr<Transform> dummy_light_transform;

		void on_window_resize();
		void build_renderables(std::shared_ptr<Scene> scene);
	};
}

