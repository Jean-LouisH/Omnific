// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <SDL.h>
#include <vector>
#include <map>

namespace Lilliputian
{
	class HumanInterfaceDevices
	{
	public:
		using ControllerButtonCode = Uint8;
		using ControllerAxisCode = Uint8;

		void detectGameControllers();
		void pollInputEvents();
		bool hasRequestedShutdown();
		bool hasRequestedCommandLine();
		void forceShutdownRequest();
		bool getHasDetectedInputChanges();

		std::vector<SDL_Haptic*> getHaptics();
		std::map<ControllerButtonCode, SDL_ControllerButtonEvent> getControllerButtonEvents();
		std::map<SDL_Keycode, SDL_KeyboardEvent> getKeyboardEvents();
		std::map<ControllerAxisCode, SDL_ControllerAxisEvent> getControllerAxisEvents();
		SDL_MouseButtonEvent getMouseButtonEvent();
		SDL_MouseMotionEvent getMouseMotionEvent();
		SDL_MouseWheelEvent  getMouseWheelEvent();
	private:

		std::map<ControllerButtonCode, SDL_ControllerButtonEvent> controllerButtonEvents;
		std::map<SDL_Keycode, SDL_KeyboardEvent> keyboardEvents;
		std::map<ControllerAxisCode, SDL_ControllerAxisEvent> controllerAxisEvents;
		SDL_MouseButtonEvent mouseButtonEvent = { 0 };
		SDL_MouseMotionEvent mouseMotionEvent = { 0 };
		SDL_MouseWheelEvent  mouseWheelEvent = { 0 };
		SDL_WindowEvent windowEvent = { 0 };
		SDL_DropEvent dropEvent = { 0 };

		std::vector<SDL_GameController*> gameControllers;
		std::vector<SDL_Haptic*> haptics;

		bool hasDetectedInputChanges = false;
		bool shutdownRequest = false;
		void clear();
	};
}