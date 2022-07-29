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

#include "input.hpp"

Omnia::Input::Input()
{
	this->controllerButtonsByString.emplace("dpad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	this->controllerButtonsByString.emplace("dpad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	this->controllerButtonsByString.emplace("dpad_up", SDL_CONTROLLER_BUTTON_DPAD_UP);
	this->controllerButtonsByString.emplace("dpad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	this->controllerButtonsByString.emplace("button_extra", SDL_CONTROLLER_BUTTON_BACK);
	this->controllerButtonsByString.emplace("button_menu", SDL_CONTROLLER_BUTTON_START);
	this->controllerButtonsByString.emplace("button_x", SDL_CONTROLLER_BUTTON_X);
	this->controllerButtonsByString.emplace("button_y", SDL_CONTROLLER_BUTTON_Y);
	this->controllerButtonsByString.emplace("button_a", SDL_CONTROLLER_BUTTON_A);
	this->controllerButtonsByString.emplace("button_b", SDL_CONTROLLER_BUTTON_B);
	this->controllerButtonsByString.emplace("left_shoulder_button", SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	this->controllerButtonsByString.emplace("left_stick_button", SDL_CONTROLLER_BUTTON_LEFTSTICK);
	this->controllerButtonsByString.emplace("right_shoulder_button", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	this->controllerButtonsByString.emplace("right_stick_button", SDL_CONTROLLER_BUTTON_RIGHTSTICK);

	this->controllerAxisEventsByString.emplace("left_axis_x", SDL_CONTROLLER_AXIS_LEFTX);
	this->controllerAxisEventsByString.emplace("left_axis_y", SDL_CONTROLLER_AXIS_LEFTY);
	this->controllerAxisEventsByString.emplace("right_axis_x", SDL_CONTROLLER_AXIS_RIGHTX);
	this->controllerAxisEventsByString.emplace("right_axis_y", SDL_CONTROLLER_AXIS_RIGHTY);
	this->controllerAxisEventsByString.emplace("left_trigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	this->controllerAxisEventsByString.emplace("right_trigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	this->keyboardEventsByString.emplace("escape", SDLK_ESCAPE);
	this->keyboardEventsByString.emplace("f1", SDLK_F1);
	this->keyboardEventsByString.emplace("f2", SDLK_F2);
	this->keyboardEventsByString.emplace("f3", SDLK_F3);
	this->keyboardEventsByString.emplace("f4", SDLK_F4);
	this->keyboardEventsByString.emplace("f5", SDLK_F5);
	this->keyboardEventsByString.emplace("f6", SDLK_F6);
	this->keyboardEventsByString.emplace("f7", SDLK_F7);
	this->keyboardEventsByString.emplace("f8", SDLK_F8);
	this->keyboardEventsByString.emplace("f9", SDLK_F9);
	this->keyboardEventsByString.emplace("f10", SDLK_F10);
	this->keyboardEventsByString.emplace("f11", SDLK_F11);
	this->keyboardEventsByString.emplace("f12", SDLK_F12);
	this->keyboardEventsByString.emplace("printscreen", SDLK_PRINTSCREEN);
	this->keyboardEventsByString.emplace("pause", SDLK_PAUSE);
	this->keyboardEventsByString.emplace("insert", SDLK_INSERT);
	this->keyboardEventsByString.emplace("delete", SDLK_DELETE);
	this->keyboardEventsByString.emplace("home", SDLK_HOME);
	this->keyboardEventsByString.emplace("page_up", SDLK_PAGEUP);
	this->keyboardEventsByString.emplace("page_down", SDLK_PAGEDOWN);
	this->keyboardEventsByString.emplace("end", SDLK_END);
	this->keyboardEventsByString.emplace("backquote", SDLK_BACKQUOTE);
	this->keyboardEventsByString.emplace("1", SDLK_1);
	this->keyboardEventsByString.emplace("2", SDLK_2);
	this->keyboardEventsByString.emplace("3", SDLK_3);
	this->keyboardEventsByString.emplace("4", SDLK_4);
	this->keyboardEventsByString.emplace("5", SDLK_5);
	this->keyboardEventsByString.emplace("6", SDLK_6);
	this->keyboardEventsByString.emplace("7", SDLK_7);
	this->keyboardEventsByString.emplace("8", SDLK_8);
	this->keyboardEventsByString.emplace("9", SDLK_9);
	this->keyboardEventsByString.emplace("0", SDLK_0);
	this->keyboardEventsByString.emplace("-", SDLK_MINUS);
	this->keyboardEventsByString.emplace("+", SDLK_PLUS);
	this->keyboardEventsByString.emplace("backspace", SDLK_BACKSPACE);
	this->keyboardEventsByString.emplace("num_lock", SDLK_NUMLOCKCLEAR);
	this->keyboardEventsByString.emplace("forward_slash", SDLK_SLASH);
	this->keyboardEventsByString.emplace("asterisk", SDLK_ASTERISK);
	this->keyboardEventsByString.emplace("keypad_minus", SDLK_KP_MINUS);
	this->keyboardEventsByString.emplace("keypad_plus", SDLK_KP_PLUS);
	this->keyboardEventsByString.emplace("keypad_enter", SDLK_KP_ENTER);
	this->keyboardEventsByString.emplace("keypad_1", SDLK_KP_1);
	this->keyboardEventsByString.emplace("keypad_2", SDLK_KP_2);
	this->keyboardEventsByString.emplace("keypad_3", SDLK_KP_3);
	this->keyboardEventsByString.emplace("keypad_4", SDLK_KP_4);
	this->keyboardEventsByString.emplace("keypad_5", SDLK_KP_5);
	this->keyboardEventsByString.emplace("keypad_6", SDLK_KP_6);
	this->keyboardEventsByString.emplace("keypad_7", SDLK_KP_7);
	this->keyboardEventsByString.emplace("keypad_8", SDLK_KP_8);
	this->keyboardEventsByString.emplace("keypad_9", SDLK_KP_9);
	this->keyboardEventsByString.emplace("keypad_0", SDLK_KP_0);
	this->keyboardEventsByString.emplace("keypad_period", SDLK_KP_PERIOD);
	this->keyboardEventsByString.emplace("tab", SDLK_TAB);
	this->keyboardEventsByString.emplace("q", SDLK_q);
	this->keyboardEventsByString.emplace("w", SDLK_w);
	this->keyboardEventsByString.emplace("e", SDLK_e);
	this->keyboardEventsByString.emplace("r", SDLK_r);
	this->keyboardEventsByString.emplace("t", SDLK_t);
	this->keyboardEventsByString.emplace("y", SDLK_y);
	this->keyboardEventsByString.emplace("u", SDLK_u);
	this->keyboardEventsByString.emplace("i", SDLK_i);
	this->keyboardEventsByString.emplace("o", SDLK_o);
	this->keyboardEventsByString.emplace("p", SDLK_p);
	this->keyboardEventsByString.emplace("a", SDLK_a);
	this->keyboardEventsByString.emplace("s", SDLK_s);
	this->keyboardEventsByString.emplace("d", SDLK_d);
	this->keyboardEventsByString.emplace("f", SDLK_f);
	this->keyboardEventsByString.emplace("g", SDLK_g);
	this->keyboardEventsByString.emplace("h", SDLK_h);
	this->keyboardEventsByString.emplace("j", SDLK_j);
	this->keyboardEventsByString.emplace("k", SDLK_k);
	this->keyboardEventsByString.emplace("l", SDLK_l);
	this->keyboardEventsByString.emplace("z", SDLK_z);
	this->keyboardEventsByString.emplace("x", SDLK_x);
	this->keyboardEventsByString.emplace("c", SDLK_c);
	this->keyboardEventsByString.emplace("v", SDLK_v);
	this->keyboardEventsByString.emplace("b", SDLK_b);
	this->keyboardEventsByString.emplace("n", SDLK_n);
	this->keyboardEventsByString.emplace("m", SDLK_m);
	this->keyboardEventsByString.emplace("left_square_bracket", SDLK_LEFTBRACKET);
	this->keyboardEventsByString.emplace("right_square_bracket", SDLK_RIGHTBRACKET);
	this->keyboardEventsByString.emplace("semicolon", SDLK_SEMICOLON);
	this->keyboardEventsByString.emplace("quote", SDLK_QUOTE);
	this->keyboardEventsByString.emplace("backslash", SDLK_BACKSLASH);
	this->keyboardEventsByString.emplace("enter", SDLK_RETURN);
	this->keyboardEventsByString.emplace("comma", SDLK_COMMA);
	this->keyboardEventsByString.emplace("period", SDLK_PERIOD);
	this->keyboardEventsByString.emplace("left_shift", SDLK_LSHIFT);
	this->keyboardEventsByString.emplace("right_shift", SDLK_RSHIFT);
	this->keyboardEventsByString.emplace("left_control", SDLK_LCTRL);
	this->keyboardEventsByString.emplace("right_control", SDLK_RCTRL);
	this->keyboardEventsByString.emplace("function", SDLK_f);
	this->keyboardEventsByString.emplace("os", SDLK_LGUI);
	this->keyboardEventsByString.emplace("left_alt", SDLK_LALT);
	this->keyboardEventsByString.emplace("space", SDLK_SPACE);
	this->keyboardEventsByString.emplace("right_alt", SDLK_RALT);
	this->keyboardEventsByString.emplace("left", SDLK_LEFT);
	this->keyboardEventsByString.emplace("up", SDLK_UP);
	this->keyboardEventsByString.emplace("right", SDLK_RIGHT);
	this->keyboardEventsByString.emplace("down", SDLK_DOWN);
}

bool Omnia::Input::isOnPress(std::string inputCode)
{
	std::vector<std::string> inputCodes;
	inputCodes.push_back(inputCode);
	return this->isOnPress(inputCodes);
}

bool Omnia::Input::isOnPress(std::vector<std::string> inputCodes)
{
	return this->isOnPress(inputCodes, 0);
}

bool Omnia::Input::isOnPress(std::vector<std::string> inputCodes, PlayerID playerID)
{
	for (int i = 0; i < inputCodes.size(); i++)
	{
		std::string inputCode = inputCodes.at(i);

		if (this->keyboardEventsByString.count(inputCode))
		{
			SDL_Keycode sdlKeyCode = this->keyboardEventsByString.at(inputCode);
			if (this->keyboardEvents.count(sdlKeyCode))
				if (this->keyboardEvents.at(sdlKeyCode).type == SDL_KEYDOWN)
					return true;
		}

		if (this->controllerButtonsByString.count(inputCode))
		{
			SDL_GameControllerButton controllerButtonCode = this->controllerButtonsByString.at(inputCode);
			if (this->controllerButtonEvents.count(controllerButtonCode))
				if (this->getControllerPlayerMap().count(playerID))
					if (this->controllerButtonEvents.at(controllerButtonCode).type == SDL_CONTROLLERBUTTONDOWN &&
						this->controllerButtonEvents.at(controllerButtonCode).which == this->getControllerPlayerMap().at(playerID))
						return true;
		}
	}

	return false;
}

bool Omnia::Input::isOnDoublePress(std::string inputCode, unsigned int timeInterval)
{
	std::vector<std::string> inputCodes;
	inputCodes.push_back(inputCode);
	return this->isOnDoublePress(inputCodes, timeInterval);
}

bool Omnia::Input::isOnDoublePress(std::vector<std::string> inputCodes, unsigned int timeInterval)
{
	return this->isOnDoublePress(inputCodes, timeInterval, 0);
}

bool Omnia::Input::isOnDoublePress(std::vector<std::string> inputCodes, unsigned int timerInterval, PlayerID playerID)
{
	for (int i = 0; i < inputCodes.size(); i++)
	{

	}

	return false;
}

bool Omnia::Input::isPressed(std::string inputCode)
{
	std::vector<std::string> inputCodes;
	inputCodes.push_back(inputCode);
	return this->isPressed(inputCodes);
}

bool Omnia::Input::isPressed(std::vector<std::string> inputCodes)
{
	return isPressed(inputCodes, 0);
}

bool Omnia::Input::isPressed(std::vector<std::string> inputCodes, PlayerID playerID)
{
	for (int i = 0; i < inputCodes.size(); i++)
	{
		std::string inputCode = inputCodes.at(i);

		if (this->keyboardEventsByString.count(inputCode))
		{
			SDL_Keycode sdlKeyCode = this->keyboardEventsByString.at(inputCode);
			if (this->heldKeys.count(sdlKeyCode))
				return true;
		}

		if (this->controllerButtonsByString.count(inputCode))
		{
			SDL_GameControllerButton controllerButtonCode = this->controllerButtonsByString.at(inputCode);
			if (this->heldControllerButtons.count(controllerButtonCode))
				if (this->getControllerPlayerMap().count(playerID))
					if (this->heldControllerButtons.at(controllerButtonCode) == this->getControllerPlayerMap().at(playerID))
						return true;
		}
	}

	return false;
}

bool Omnia::Input::isOnRelease(std::string inputCode)
{
	std::vector<std::string> inputCodes;
	inputCodes.push_back(inputCode);
	return this->isOnRelease(inputCodes);
}

bool Omnia::Input::isOnRelease(std::vector<std::string> inputCodes)
{
	return this->isOnRelease(inputCodes, 0);
}

bool Omnia::Input::isOnRelease(std::vector<std::string> inputCodes, PlayerID playerID)
{
	for (int i = 0; i < inputCodes.size(); i++)
	{
		std::string inputCode = inputCodes.at(i);

		if (this->keyboardEventsByString.count(inputCode))
		{
			SDL_Keycode sdlKeyCode = this->keyboardEventsByString.at(inputCode);
			if (this->keyboardEvents.count(sdlKeyCode))
				if (this->keyboardEvents.at(sdlKeyCode).type == SDL_KEYUP)
					return true;
		}

		if (this->controllerButtonsByString.count(inputCode))
		{
			SDL_GameControllerButton controllerButtonCode = this->controllerButtonsByString.at(inputCode);
			if (this->controllerButtonEvents.count(controllerButtonCode))
				if (this->getControllerPlayerMap().count(playerID))
					if (this->controllerButtonEvents.at(controllerButtonCode).type == SDL_CONTROLLERBUTTONUP &&
						this->controllerButtonEvents.at(controllerButtonCode).which == this->getControllerPlayerMap().at(playerID))
						return true;
		}
	}

	return false;
}

bool Omnia::Input::isReleased(std::string inputCode)
{
	return !this->isPressed(inputCode);
}

bool Omnia::Input::isReleased(std::string inputCode, PlayerID playerID)
{
	std::vector<std::string> inputCodes;
	inputCodes.push_back(inputCode);
	return !this->isPressed(inputCodes, playerID);
}

float Omnia::Input::getAxis(std::string inputCode)
{
	return this->getAxis(inputCode, 0);
}

float Omnia::Input::getAxis(std::string inputCode, PlayerID playerID)
{
	float axis = 0.0;

	if (this->controllerAxisEventsByString.count(inputCode))
	{
		SDL_GameControllerAxis controllerAxisCode = this->controllerAxisEventsByString.at(inputCode);
		if (controllerAxisEvents.count(controllerAxisCode))
			if (this->getControllerPlayerMap().count(playerID))
				if (controllerAxisEvents.at(controllerAxisCode).which == this->getControllerPlayerMap().at(playerID))
					axis = (double)(controllerAxisEvents.at(controllerAxisCode).value) / pow(2.0, 15.0);
	}

	return axis;
}

bool Omnia::Input::isLeftMouseButtonOnPress()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_LEFT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONDOWN;
}

bool Omnia::Input::isLeftMouseButtonOnRelease()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_LEFT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP;
}

bool Omnia::Input::isLeftMouseButtonDoubleClicked()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_LEFT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP &&
		this->mouseButtonEvent.clicks == 2;
}

bool Omnia::Input::isMiddleMouseButtonOnPress()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_MIDDLE &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONDOWN;
}

bool Omnia::Input::isMiddleMouseButtonOnRelease()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_MIDDLE &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP;
}

bool Omnia::Input::isMiddleMouseButtonDoubleClicked()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_MIDDLE &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP &&
		this->mouseButtonEvent.clicks == 2;
}

bool Omnia::Input::isRightMouseButtonOnPress()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_RIGHT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONDOWN;
}

bool Omnia::Input::isRightMouseButtonOnRelease()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_RIGHT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP;
}

bool Omnia::Input::isRightMouseButtonDoubleClicked()
{
	return this->mouseButtonEvent.button == SDL_BUTTON_RIGHT &&
		this->mouseButtonEvent.type == SDL_MOUSEBUTTONUP &&
		this->mouseButtonEvent.clicks == 2;
}

glm::vec2 Omnia::Input::getMousePosition()
{
	glm::vec2 vector2;
	vector2.x = this->mouseMotionEvent.x;
	vector2.y = this->mouseMotionEvent.y;
	return vector2;
}

glm::vec2 Omnia::Input::getMouseWheelVelocity()
{
	glm::vec2 vector2;
	vector2.x = this->mouseWheelEvent.x;
	vector2.y = this->mouseWheelEvent.y;
	return vector2;
}

glm::vec2 Omnia::Input::getMouseMotionVelocity()
{
	glm::vec2 vector2;
	vector2.x = this->mouseMotionEvent.xrel;
	vector2.y = this->mouseMotionEvent.yrel;
	return vector2;
}

bool Omnia::Input::isDropFileDetected()
{
	return this->dropEvent.type == SDL_DROPFILE;
}

std::string Omnia::Input::getDropFilePath()
{
	return this->dropEvent.file;
}

uint32_t Omnia::Input::getDropFileWindowID()
{
	uint32_t dropFileWindowID = -1;

	if (this->isDropFileDetected())
		dropFileWindowID = this->dropEvent.windowID;

	return dropFileWindowID;
}

void Omnia::Input::clear()
{
	this->controllerAxisEvents.clear();
	this->controllerButtonEvents.clear();
	this->keyboardEvents.clear();

	if (dropEvent.file != NULL)
	{
		SDL_free(dropEvent.file);
		dropEvent = { 0 };
	}

	this->hasDetectedInputChanges = false;
}

void Omnia::Input::detectGameControllers()
{
	if (SDL_NumJoysticks() != this->gameControllers.size())
	{
		for (int i = 0; i < this->gameControllers.size(); ++i)
		{
			SDL_GameControllerClose(this->gameControllers.at(i));
			SDL_HapticClose(this->haptics.at(i));
		}

		this->gameControllers.clear();
		this->haptics.clear();
		this->controllerPlayerMap.clear();

		for (int i = 0; i < SDL_NumJoysticks(); ++i)
		{
			if (SDL_IsGameController(i))
			{
				SDL_GameController* newController = SDL_GameControllerOpen(i);
				SDL_Joystick* joystick = SDL_GameControllerGetJoystick(newController);
				this->controllerPlayerMap.emplace(i, SDL_JoystickInstanceID(joystick));
				this->gameControllers.push_back(newController);
				this->haptics.push_back(SDL_HapticOpenFromJoystick(joystick));
				this->newlyLoadedPlayerIDs.push(i);
				SDL_HapticRumbleInit(this->haptics.back());
				if (!this->gameControllers.back())
					fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

void Omnia::Input::pollInputEvents()
{
	SDL_Event SDLEvents;

	this->clear();
	this->hasDetectedInputChanges = false;

	while (SDL_PollEvent(&SDLEvents))
	{
		switch (SDLEvents.type)
		{
		case SDL_QUIT:
			this->shutdownRequest = true;
			break;

		case SDL_KEYDOWN:
			this->heldKeys.insert(SDLEvents.key.keysym.sym);
			this->keyboardEvents.emplace(SDLEvents.key.keysym.sym, SDLEvents.key);
			this->hasDetectedInputChanges = true;
			break;
		case SDL_KEYUP:
			this->heldKeys.erase(SDLEvents.key.keysym.sym);
			this->keyboardEvents.emplace(SDLEvents.key.keysym.sym, SDLEvents.key);
			this->hasDetectedInputChanges = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			this->mouseButtonEvent = SDLEvents.button;
			this->hasDetectedInputChanges = true;
			break;

		case SDL_MOUSEMOTION:
			this->mouseMotionEvent = SDLEvents.motion;
			break;

		case SDL_MOUSEWHEEL:
			this->mouseWheelEvent = SDLEvents.wheel;
			this->hasDetectedInputChanges = true;
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			this->heldControllerButtons.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton.which);
			this->controllerButtonEvents.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton);
			this->hasDetectedInputChanges = true;
			break;
		case SDL_CONTROLLERBUTTONUP:
			this->heldControllerButtons.erase((SDL_GameControllerButton)SDLEvents.cbutton.button);
			this->controllerButtonEvents.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton);
			this->hasDetectedInputChanges = true;
			break;

		case SDL_CONTROLLERAXISMOTION:
			this->controllerAxisEvents.emplace((SDL_GameControllerAxis)SDLEvents.caxis.axis, SDLEvents.caxis);
			this->hasDetectedInputChanges = true;
			break;

		case SDL_DROPFILE:
			this->dropEvent = SDLEvents.drop;
			break;
		}
	}
}

bool Omnia::Input::hasRequestedShutdown()
{
	return this->shutdownRequest;
}

bool Omnia::Input::hasRequestedRestart()
{
	return this->restartRequest;
}

void Omnia::Input::requestShutdown()
{
	this->shutdownRequest = true;
}

void Omnia::Input::requestRestart()
{
	this->restartRequest = true;
}

bool Omnia::Input::getHasDetectedInputChanges()
{
	return this->hasDetectedInputChanges;
}

bool Omnia::Input::hasRequestedCommandLine()
{
	bool backquoteReleased = false;

	if (this->keyboardEvents.count(SDLK_BACKQUOTE))
		backquoteReleased = this->keyboardEvents.at(SDLK_BACKQUOTE).type == SDL_KEYUP;

	return backquoteReleased;	 
}

std::vector<SDL_Haptic*> Omnia::Input::getHaptics()
{
	return this->haptics;
}

std::unordered_map<Omnia::PlayerID, SDL_JoystickID> Omnia::Input::getControllerPlayerMap()
{
	return this->controllerPlayerMap;
}

std::queue<Omnia::PlayerID>& Omnia::Input::getNewlyLoadedPlayerIDs()
{
	return this->newlyLoadedPlayerIDs;
}