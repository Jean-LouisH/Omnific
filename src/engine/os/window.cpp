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

Lilliputian::Window::Window(const char* title, uint16_t width, uint16_t height, bool isFullscreen)
{
	this->isFullscreen = isFullscreen;
	this->sdlWindow = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN_DESKTOP & isFullscreen);

	SDL_DisableScreenSaver();
	SDL_GetCurrentDisplayMode(0, this->sdlDisplayMode);
}


void Lilliputian::Window::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowFullscreen(this->sdlWindow, 0);
	SDL_SetWindowSize(this->sdlWindow, width_px, height_px);

}

void Lilliputian::Window::setToFullscreen()
{
	SDL_SetWindowDisplayMode(this->sdlWindow, this->sdlDisplayMode);
	SDL_SetWindowFullscreen(this->sdlWindow, SDL_WINDOW_FULLSCREEN);
}

void Lilliputian::Window::toggleWindowedFullscreen()
{
	this->isFullscreen = !isFullscreen;

	if (this->isFullscreen)
	{
		SDL_SetWindowFullscreen(this->sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(this->sdlWindow, 0);
		SDL_SetWindowPosition(
			this->sdlWindow,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED);
	}

	SDL_ShowCursor(!this->isFullscreen);
}

void Lilliputian::Window::resize(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowSize(this->sdlWindow, width_px, height_px);
	SDL_SetWindowPosition(this->sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void Lilliputian::Window::changeTitle(const char* title)
{
	SDL_SetWindowTitle(this->sdlWindow, title);
}

void Lilliputian::Window::changeIcon(Image image)
{
	SDL_SetWindowIcon(this->sdlWindow, image.getSDLSurface());
}

void Lilliputian::Window::maximize()
{
	SDL_MaximizeWindow(this->sdlWindow);
}

void Lilliputian::Window::minimize()
{
	SDL_MinimizeWindow(this->sdlWindow);
}

void Lilliputian::Window::raise()
{
	SDL_RaiseWindow(this->sdlWindow);
}

void Lilliputian::Window::restore()
{
	SDL_RestoreWindow(this->sdlWindow);
}

void Lilliputian::Window::hide()
{
	SDL_HideWindow(this->sdlWindow);
}

void Lilliputian::Window::show()
{
	SDL_ShowWindow(this->sdlWindow);
}

void Lilliputian::Window::sleep(int time_ms)
{
	if (time_ms > 0)
		SDL_Delay(time_ms);
}

SDL_Window* Lilliputian::Window::getSDLWindow()
{
	return this->sdlWindow;
}