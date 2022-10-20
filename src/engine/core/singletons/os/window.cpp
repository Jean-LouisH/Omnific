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

#include "window.hpp"
#include <stdint.h>
#include "core/singletons/os/os.hpp"

void Omnia::Window::initialize(std::string title, uint16_t width, uint16_t height, bool isFullscreen, std::string renderingContext)
{
	uint64_t renderingContextFlag = 0x0;

	if (renderingContext == "opengl")
		renderingContextFlag = SDL_WINDOW_OPENGL;

	this->sdlWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN_DESKTOP & isFullscreen | renderingContextFlag), SDL_DestroyWindow);

	SDL_DisableScreenSaver();
	SDL_GetCurrentDisplayMode(0, this->sdlDisplayMode.get());
	this->isFullscreen = isFullscreen;
}


void Omnia::Window::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	if (this->sdlWindow != nullptr)
	{
		SDL_SetWindowFullscreen(this->sdlWindow.get(), 0);
		SDL_SetWindowSize(this->sdlWindow.get(), width_px, height_px);
		OS::getLogger().write("Set Window to windowed.");
	}
}

void Omnia::Window::setToFullscreen()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_SetWindowDisplayMode(this->sdlWindow.get(), this->sdlDisplayMode.get());
		SDL_SetWindowFullscreen(this->sdlWindow.get(), SDL_WINDOW_FULLSCREEN);
		OS::getLogger().write("Set Window to fullscreen.");
	}
}

void Omnia::Window::toggleWindowedFullscreen()
{
	if (this->sdlWindow != nullptr)
	{
		this->isFullscreen = !isFullscreen;

		if (this->isFullscreen)
		{
			SDL_SetWindowFullscreen(this->sdlWindow.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
			OS::getLogger().write("Set Window to windowed fullscreen.");
		}
		else
		{
			SDL_SetWindowFullscreen(this->sdlWindow.get(), 0);
			SDL_SetWindowPosition(
				this->sdlWindow.get(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED);
			OS::getLogger().write("Set Window to windowed.");
		}

		SDL_ShowCursor(!this->isFullscreen);
	}
}

void Omnia::Window::resize(uint16_t width_px, uint16_t height_px)
{
	if (this->sdlWindow != nullptr)
	{
		SDL_SetWindowSize(this->sdlWindow.get(), width_px, height_px);
		SDL_SetWindowPosition(this->sdlWindow.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		OS::getLogger().write("Resized Window to: " + std::to_string(width_px) + "x" + std::to_string(height_px));
	}
}

void Omnia::Window::changeTitle(const char* title)
{
	if (this->sdlWindow != nullptr)
	{
		SDL_SetWindowTitle(this->sdlWindow.get(), title);
		OS::getLogger().write("Changed Window title to: \"" + (std::string)title + "\"");
	}
}

void Omnia::Window::changeIcon(void* data, uint32_t width, uint32_t height, uint32_t depth, uint32_t pitch)
{
	if (this->sdlWindow != nullptr)
	{
		SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceFrom(
			data,
			width,
			height,
			depth,
			pitch,
			0,
			0,
			0,
			0);

		if (sdlSurface != nullptr)
		{
			SDL_SetWindowIcon(this->sdlWindow.get(), sdlSurface);
			SDL_FreeSurface(sdlSurface);
			OS::getLogger().write("Changed Window icon.");
		}
	}
}

void Omnia::Window::maximize()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_MaximizeWindow(this->sdlWindow.get());
		OS::getLogger().write("Maximized Window.");
	}
}

void Omnia::Window::minimize()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_MinimizeWindow(this->sdlWindow.get());
		OS::getLogger().write("Minimized Window.");
	}
}

void Omnia::Window::raise()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_RaiseWindow(this->sdlWindow.get());
		OS::getLogger().write("Raised Window.");
	}
}

void Omnia::Window::restore()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_RestoreWindow(this->sdlWindow.get());
		OS::getLogger().write("Restored Window.");
	}
}

void Omnia::Window::hide()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_HideWindow(this->sdlWindow.get());
		OS::getLogger().write("Hid Window.");
	}
}

void Omnia::Window::show()
{
	if (this->sdlWindow != nullptr)
	{
		SDL_ShowWindow(this->sdlWindow.get());
		OS::getLogger().write("Showed Window.");
	}
}

Omnia::Rectangle Omnia::Window::getWindowSize()
{
	Rectangle rectangle;

	if (this->sdlWindow != nullptr)
	{
		int width = 0;
		int height = 0;

		SDL_GetWindowSize(this->sdlWindow.get(), &width, &height);
		rectangle.width = width;
		rectangle.height = height;
	}

	return rectangle;
}

SDL_Window* Omnia::Window::getSDLWindow()
{
	return this->sdlWindow.get();
}