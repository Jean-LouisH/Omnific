#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"
#include "Utilities/Collections/Map.hpp"

namespace Lilliputian
{
	class OSWindowInputs
	{
	private:
	public:
		using ControllerButtonCode = Uint8;
		using ControllerAxisCode = Uint8;

		Multimap<ControllerButtonCode, SDL_ControllerButtonEvent> controllerButtonEvents;
		Map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents;
		Multimap<ControllerAxisCode, SDL_ControllerAxisEvent> controllerAxisEvents;
		SDL_MouseButtonEvent mouseButtonEvent = { 0 };
		SDL_MouseMotionEvent mouseMotionEvent = { 0 };
		SDL_MouseWheelEvent  mouseWheelEvent = { 0 };
		SDL_WindowEvent windowEvent = { 0 };

		void clear();
	};
}