#pragma once

#include <SDL.h>
#include "Utilities/Collections/Map.hpp"
#include "Utilities/Collections/Vector.hpp"
#include "Utilities/Collections/Queue.hpp"

namespace Lilliputian
{
	class OSWindow
	{
	private:
		SDL_Window* sdlWindow;

		typedef struct KeyEvent
		{
			Uint16 mod;
			Uint32 timestamp_ms;
		}KeyEvent;

		typedef struct MouseState
		{
			Uint32 type;
			Uint8 clicks;
			Sint32 x;
			Sint32 y;
			Sint32 xrel;
			Sint32 yrel;
			Sint32 xScroll;
			Sint32 yScroll;
			Uint32 direction;
			Uint8 button;
			Uint8 buttonState;
			Uint32 motionState;
			Uint32 timestamp_ms;
			Uint32 windowID;

			MouseState() :
				type(0),
				clicks(0),
				x(0),
				y(0),
				xrel(0),
				yrel(0),
				xScroll(0),
				yScroll(0),
				direction(0),
				button(0),
				buttonState(0),
				motionState(0),
				timestamp_ms(0),
				windowID(0)
			{}
		}MouseState;

		bool shutdownRequest;
		Map<SDL_Keycode, KeyEvent> pressedKeys;
		Map<SDL_Keycode, KeyEvent> releasedKeys;
		MouseState mouse;
		Vector<SDL_GameController*> gameControllers;
		Vector<SDL_Haptic*> haptics;
		Vector<SDL_HapticEffect> hapticEffects;

	public:
		OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen);
		void sleep(int time_ms);
		void detectGameControllers();
		void pollInputEvents();
		bool hasRequestedShutdown();
		SDL_Window* getSDLWindow();
	};
}