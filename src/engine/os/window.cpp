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

Esi::Window::Window(std::string title, uint16_t width, uint16_t height, bool isFullscreen, std::string renderingContext)
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


void Esi::Window::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowFullscreen(this->sdlWindow.get(), 0);
	SDL_SetWindowSize(this->sdlWindow.get(), width_px, height_px);

}

void Esi::Window::setToFullscreen()
{
	SDL_SetWindowDisplayMode(this->sdlWindow.get(), this->sdlDisplayMode.get());
	SDL_SetWindowFullscreen(this->sdlWindow.get(), SDL_WINDOW_FULLSCREEN);
}

void Esi::Window::toggleWindowedFullscreen()
{
	this->isFullscreen = !isFullscreen;

	if (this->isFullscreen)
	{
		SDL_SetWindowFullscreen(this->sdlWindow.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(this->sdlWindow.get(), 0);
		SDL_SetWindowPosition(
			this->sdlWindow.get(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED);
	}

	SDL_ShowCursor(!this->isFullscreen);
}

void Esi::Window::resize(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowSize(this->sdlWindow.get(), width_px, height_px);
	SDL_SetWindowPosition(this->sdlWindow.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Esi::Window::changeTitle(const char* title)
{
	SDL_SetWindowTitle(this->sdlWindow.get(), title);
}

void Esi::Window::changeIcon(Image image)
{
	SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceFrom(
		image.getData(),
		image.getWidth(),
		image.getHeight(),
		image.getDepth(),
		image.getPitch(),
		0,
		0,
		0,
		0);

	if (sdlSurface != nullptr)
	{
		SDL_SetWindowIcon(this->sdlWindow.get(), sdlSurface);
		SDL_FreeSurface(sdlSurface);
	}
}

void Esi::Window::maximize()
{
	SDL_MaximizeWindow(this->sdlWindow.get());
}

void Esi::Window::minimize()
{
	SDL_MinimizeWindow(this->sdlWindow.get());
}

void Esi::Window::raise()
{
	SDL_RaiseWindow(this->sdlWindow.get());
}

void Esi::Window::restore()
{
	SDL_RestoreWindow(this->sdlWindow.get());
}

void Esi::Window::hide()
{
	SDL_HideWindow(this->sdlWindow.get());
}

void Esi::Window::show()
{
	SDL_ShowWindow(this->sdlWindow.get());
}

void Esi::Window::sleep(int time_ms)
{
	if (time_ms > 0)
		SDL_Delay(time_ms);
}

Esi::Rectangle Esi::Window::getWindowSize()
{
	Rectangle rectangle;
	int width = 0;
	int height = 0;
	
	SDL_GetWindowSize(this->sdlWindow.get(), &width, &height);
	rectangle.width = width;
	rectangle.height = height;

	return rectangle;
}

SDL_Window* Esi::Window::getSDLWindow()
{
	return this->sdlWindow.get();
}