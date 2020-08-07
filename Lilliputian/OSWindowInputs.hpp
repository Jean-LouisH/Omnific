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
		Vector<SDL_ControllerAxisEvent> controllerAxisEvents;
		Vector<SDL_ControllerButtonEvent> controllerButtonEvents;
		Vector<SDL_KeyboardEvent> keyboardEvents;
		Vector<SDL_MouseButtonEvent> mouseButtonEvents;
		Vector<SDL_MouseMotionEvent> mouseMotionEvents;
		Vector<SDL_MouseWheelEvent>  mouseWheelEvents;

		void clear();
	};
}