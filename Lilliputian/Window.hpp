#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"
#include "Utilities/Collections/Map.hpp"

namespace Lilliputian
{
	class Window
	{
	private:
		SDL_Window* sdlWindow;
		bool isFullscreen;
	public:
		Window(const char* title, uint16_t width, uint16_t height, bool isFullscreen);
		void setToWindowed(uint16_t width_px, uint16_t height_px);
		void setToFullscreen(SDL_DisplayMode* mode);
		void toggleWindowedFullscreen();
		void sleep(int time_ms);

		SDL_Window* getSDLWindow();
	};
}