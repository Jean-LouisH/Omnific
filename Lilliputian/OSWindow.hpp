#pragma once

#include <SDL.h>

namespace Lilliputian
{
	class OSWindow
	{
	private:
		SDL_Window* sdlWindow;
	public:
		OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen);
		void sleep(int time_ms);
	};
}