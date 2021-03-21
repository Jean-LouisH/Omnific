//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once

#include <stdint.h>
#include "utilities/collections/map.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class InputAPI
	{
	public:
		enum inputCodes
		{
			GAME_CONTROLLER_BUTTON_DPAD_LEFT,
			GAME_CONTROLLER_BUTTON_DPAD_RIGHT,
			GAME_CONTROLLER_BUTTON_DPAD_UP,
			GAME_CONTROLLER_BUTTON_DPAD_DOWN,
			GAME_CONTROLLER_BUTTON_EXTRA,
			GAME_CONTROLLER_BUTTON_MENU,
			GAME_CONTROLLER_BUTTON_X,
			GAME_CONTROLLER_BUTTON_Y,
			GAME_CONTROLLER_BUTTON_A,
			GAME_CONTROLLER_BUTTON_B,
			GAME_CONTROLLER_BUTTON_LEFT_SHOULDER,
			GAME_CONTROLLER_BUTTON_LEFT_STICK_BUTTON,
			GAME_CONTROLLER_BUTTON_RIGHT_SHOULDER,
			GAME_CONTROLLER_BUTTON_RIGHT_STICK_BUTTON,
			GAME_CONTROLLER_LEFT_AXIS_LEFT,
			GAME_CONTROLLER_LEFT_AXIS_RIGHT,
			GAME_CONTROLLER_LEFT_AXIS_UP,
			GAME_CONTROLLER_LEFT_AXIS_DOWN,
			GAME_CONTROLLER_RIGHT_AXIS_LEFT,
			GAME_CONTROLLER_RIGHT_AXIS_RIGHT,
			GAME_CONTROLLER_RIGHT_AXIS_UP,
			GAME_CONTROLLER_RIGHT_AXIS_DOWN,
			GAME_CONTROLLER_TRIGGER_LEFT_AXIS,
			GAME_CONTROLLER_TRIGGER_RIGHT_AXIS,
			KEY_ESCAPE,
			KEY_F1,
			KEY_F2,
			KEY_F3,
			KEY_F4,
			KEY_F5,
			KEY_F6,
			KEY_F7,
			KEY_F8,
			KEY_F9,
			KEY_F10,
			KEY_F11,
			KEY_F12,
			KEY_PRINTSCREEN,
			KEY_PAUSE,
			KEY_INSERT,
			KEY_DELETE,
			KEY_HOME,
			KEY_PAGE_UP,
			KEY_PAGE_DOWN,
			KEY_END,
			KEY_BACKQUOTE,
			KEY_1,
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,
			KEY_0,
			KEY_MINUS,
			KEY_PLUS,
			KEY_BACKSPACE,
			KEY_NUM_LOCK,
			KEY_KP_FORWARD_SLASH,
			KEY_KP_ASTERISK,
			KEY_KP_MINUS,
			KEY_KP_PLUS,
			KEY_KP_ENTER,
			KEY_KP_1,
			KEY_KP_2,
			KEY_KP_3,
			KEY_KP_4,
			KEY_KP_5,
			KEY_KP_6,
			KEY_KP_7,
			KEY_KP_8,
			KEY_KP_9,
			KEY_KP_0,
			KEY_KP_PERIOD,
			KEY_TAB,
			KEY_Q,
			KEY_W,
			KEY_E,
			KEY_R,
			KEY_T,
			KEY_Y,
			KEY_U,
			KEY_I,
			KEY_O,
			KEY_P,
			KEY_A,
			KEY_S,
			KEY_D,
			KEY_F,
			KEY_G,
			KEY_H,
			KEY_J,
			KEY_K,
			KEY_L,
			KEY_Z,
			KEY_X,
			KEY_C,
			KEY_V,
			KEY_B,
			KEY_N,
			KEY_M,
			KEY_OPEN_SQUARE_BRACKET,
			KEY_CLOSED_SQUARE_BRACKET,
			KEY_SEMICOLON,
			KEY_QUOTE,
			KEY_BACKSLASH,
			KEY_ENTER,
			KEY_COMMA,
			KEY_PERIOD,
			KEY_FORWARD,
			KEY_LEFT_SHIFT,
			KEY_RIGHT_SHIFT,
			KEY_LEFT_CONTROL,
			KEY_FUNCTION,
			KEY_OS,
			KEY_LEFT_ALT,
			KEY_SPACEBAR,
			KEY_RIGHT_ALT,
			KEY_RIGHT_CONTROL,
			KEY_LEFT_ARROW,
			KEY_UP_ARROW,
			KEY_RIGHT_ARROW,
			KEY_DOWN_ARROW,
			MOUSE_WHEEL_NORMAL,
			MOUSE_WHEEL_FLIPPED,
			MOUSE_BUTTON_LEFT,
			MOUSE_BUTTON_MIDDLE,
			MOUSE_BUTTON_RIGHT
		};

		uint16_t analogueStickSensitivity;

		InputAPI();
		void insertActionInput(const char* actionName, uint8_t inputCode);
		void replaceActionInput(const char* actionName, uint8_t inputCode);
		void removeActionInput(const char* actionName, uint8_t inputCode);
		void removeAllActionInputs(const char* actionName);
		Map<String, Vector<uint8_t>> getActionInputs();
		bool isKeyOnPress(unsigned int keyCode);
		bool isKeyOnDoublePress(unsigned int keyCode, unsigned int timeInterval_ms);
		bool isKeyOnRelease(unsigned int keyCode);
		bool isKeyOnHold(unsigned int keyCode);
		bool isActionOnPress(const char* actionName);
		bool isActionOnDoublePress(const char* actionName, unsigned int timeInterval_ms);
		bool isActionOnRelease(const char* actionName);
		bool isActionOnHold(const char* actionName, unsigned int timeInterval_ms);
		float getAxisStrength(const char* axisActionName);
	private:
		Map<String, Vector<uint8_t>> actionInputs;
	};
}