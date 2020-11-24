#include "Window.hpp"

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
}


void Lilliputian::Window::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowFullscreen(this->sdlWindow, 0);
	SDL_SetWindowSize(this->sdlWindow, width_px, height_px);

}

void Lilliputian::Window::setToFullscreen(SDL_DisplayMode* mode)
{
	SDL_SetWindowDisplayMode(this->sdlWindow, mode);
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

void Lilliputian::Window::resizeWindow(uint16_t width_px, uint16_t height_px)
{

}

void Lilliputian::Window::changeTitle(const char* title)
{

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