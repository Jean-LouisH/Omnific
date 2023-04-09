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
#include <memory>
#include <map>
#include <core/singletons/os/os.hpp>

namespace Omnia
{
	/* Processes RenderableComponents for output to graphics display. */
	class PathTraceRenderingSystem : public System
	{
	public:
		PathTraceRenderingSystem()
		{
			SDL_InitSubSystem(SDL_INIT_VIDEO);

			OS::createWindow("",
				640,
				480,
				false,
				this->getRenderingBackendName());

			this->type = TYPE_STRING;
		}
		~PathTraceRenderingSystem();

		static constexpr const char* TYPE_STRING = "PathTraceRenderingSystem";

		virtual Registerable* instance() override
		{
			return new PathTraceRenderingSystem(*this);
		}

		virtual void initializeOutput() override;
		virtual void onOutput(std::shared_ptr<Scene> scene) override;
		virtual void finalizeOutput() override;
		std::string getRenderingBackendName();
	private:
		SceneID activeSceneID = 0;
	};
}

