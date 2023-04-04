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
#include <vector>
#include <unordered_map>
#include <core/assets/image.hpp>
#include <memory>
#include <engine_api.hpp>
#include <glm/glm.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API Window
	{
	public:
		void initialize(std::string title, uint16_t width, uint16_t height, bool isFullscreen, std::string renderingContext);
		void initializeWindowContext(std::string renderingContext);
		void setToWindowed(uint16_t width, uint16_t height);
		void setToFullscreen();
		void toggleWindowedFullscreen();
		void resize(uint16_t width, uint16_t height);
		void changeTitle(const char* title);
		void changeIcon(void* data, uint32_t width, uint32_t height, uint32_t depth, uint32_t pitch);
		void maximize();
		void minimize();
		void raise();
		void restore();
		void hide();
		void show();
		glm::vec2 getWindowSize();

		SDL_Window* getSDLWindow();
		SDL_GLContext getSDLGLContext();
	private:
		std::shared_ptr<SDL_Window> sdlWindow = {nullptr, SDL_DestroyWindow};
		SDL_GLContext sdlGlContext;
		std::shared_ptr<SDL_DisplayMode> sdlDisplayMode;
		bool isFullscreen;
	};
}