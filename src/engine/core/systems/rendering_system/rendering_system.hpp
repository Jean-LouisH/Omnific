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
#include "entity_renderable.hpp"
#include "renderable_layer.hpp"
#include <memory>
#include <map>
#include <core/singletons/os/os.hpp>

namespace Omnia
{
	/* Processes RenderableComponents for output to graphics display. */
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
		virtual void onLate(std::shared_ptr<Scene> scene) override;
		virtual void finalize() override;
		std::string getRenderingBackendName();
	private:
		std::shared_ptr<OpenGLRenderingBackend> openglBackend;
		std::vector<std::vector<RenderableLayer>> renderableLayerLists;
		SceneID activeSceneID = 0;
		glm::vec2 lastDetectedWindowSize;

		std::shared_ptr<Light> dummyLight;
		std::shared_ptr<Transform> dummyLightTransform;

		void onWindowResize();
		void buildRenderables(std::shared_ptr<Scene> scene);
	};
}

