#include "OSWindow.hpp"

Lilliputian::OSWindow::OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen)
{
	this->sdlWindow = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN_DESKTOP & isFullscreen);
}

void Lilliputian::OSWindow::sleep(int time_ms)
{
	if (time_ms > 0)
		SDL_Delay(time_ms);
}