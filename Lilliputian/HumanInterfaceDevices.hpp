#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"
#include "Utilities/Collections/Map.hpp"

namespace Lilliputian
{
	class HumanInterfaceDevices
	{
		using ControllerButtonCode = Uint8;
		using ControllerAxisCode = Uint8;
	public:
		void detectGameControllers();
		void pollInputEvents();
		bool hasRequestedShutdown();

		Vector<SDL_Haptic*> getHaptics();
	private:
		Multimap<ControllerButtonCode, SDL_ControllerButtonEvent> controllerButtonEvents;
		Map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents;
		Multimap<ControllerAxisCode, SDL_ControllerAxisEvent> controllerAxisEvents;
		SDL_MouseButtonEvent mouseButtonEvent = { 0 };
		SDL_MouseMotionEvent mouseMotionEvent = { 0 };
		SDL_MouseWheelEvent  mouseWheelEvent = { 0 };
		SDL_WindowEvent windowEvent = { 0 };

		Vector<SDL_GameController*> gameControllers;
		Vector<SDL_Haptic*> haptics;

		bool shutdownRequest = false;
		void clear();
	};
}