#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"
#include "OSWindowInputs.hpp"

namespace Lilliputian
{
	class OSWindow
	{
	private:
		SDL_Window* sdlWindow;

		bool shutdownRequest;
		bool isFullscreen;
		OSWindowInputs inputs;
		Vector<SDL_GameController*> gameControllers;
		Vector<SDL_Haptic*> haptics;

	public:
		OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen);
		void sleep(int time_ms);
		void detectGameControllers();
		void pollInputEvents();
		bool hasRequestedShutdown();
		void setToWindowed(uint16_t width_px, uint16_t height_px);
		void setToFullscreen(SDL_DisplayMode* mode);
		void toggleWindowedFullscreen();
		SDL_Window* getSDLWindow();
		OSWindowInputs getInputs();
		Vector<SDL_Haptic*> getHaptics();
	};
}