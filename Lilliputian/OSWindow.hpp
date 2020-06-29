#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"

namespace Lilliputian
{
	class OSWindow
	{
	private:
		SDL_Window* sdlWindow;

		bool shutdownRequest;
		bool isFullscreen;

		Vector<SDL_ControllerAxisEvent> controllerAxisEvents;
		Vector<SDL_ControllerButtonEvent> controllerButtonEvents;
		Vector<SDL_KeyboardEvent> keyboardEvents;
		Vector<SDL_MouseButtonEvent> mouseButtonEvents;
		Vector<SDL_MouseMotionEvent> mouseMotionEvents;
		Vector<SDL_MouseWheelEvent>  mouseWheelEvents;

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
		Vector<SDL_ControllerAxisEvent> getControllerAxisEvents();
		Vector<SDL_ControllerButtonEvent> getControllerButtonEvents();
		Vector<SDL_KeyboardEvent> getKeyboardEvents();
		Vector<SDL_MouseButtonEvent> getMouseButtonEvents();
		Vector<SDL_MouseMotionEvent> getMouseMotionEvents();
		Vector<SDL_MouseWheelEvent>  getMouseWheelEvents();
		Vector<SDL_Haptic*> getHaptics();
	};
}