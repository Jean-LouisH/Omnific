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

#include "human_interface_devices.hpp"

void Esi::HumanInterfaceDevices::clear()
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

void Esi::HumanInterfaceDevices::detectGameControllers()
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

void Esi::HumanInterfaceDevices::pollInputEvents()
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
		case SDL_KEYUP:
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
		case SDL_CONTROLLERBUTTONUP:
			this->controllerButtonEvents.emplace(SDLEvents.cbutton.button, SDLEvents.cbutton);
			this->hasDetectedInputChanges = true;
			break;

		case SDL_CONTROLLERAXISMOTION:
			this->controllerAxisEvents.emplace(SDLEvents.caxis.axis, SDLEvents.caxis);
			this->hasDetectedInputChanges = true;
			break;

		case SDL_DROPFILE:
			this->dropEvent = SDLEvents.drop;
			break;
		}
	}
}

bool Esi::HumanInterfaceDevices::hasRequestedShutdown()
{
	return this->shutdownRequest;
}

bool Esi::HumanInterfaceDevices::hasRequestedRestart()
{
	return this->restartRequest;
}

void Esi::HumanInterfaceDevices::forceShutdownRequest()
{
	this->shutdownRequest = true;
}

void Esi::HumanInterfaceDevices::forceRestartRequest()
{
	this->restartRequest = true;
}

bool Esi::HumanInterfaceDevices::getHasDetectedInputChanges()
{
	return this->hasDetectedInputChanges;
}

bool Esi::HumanInterfaceDevices::hasRequestedCommandLine()
{
	bool backquoteReleased = false;

	if (this->keyboardEvents.count(SDLK_BACKQUOTE))
		backquoteReleased = this->keyboardEvents.at(SDLK_BACKQUOTE).type == SDL_KEYUP;

	return backquoteReleased;	 
}

std::vector<SDL_Haptic*> Esi::HumanInterfaceDevices::getHaptics()
{
	return this->haptics;
}

std::unordered_map<Esi::HumanInterfaceDevices::ControllerButtonCode, SDL_ControllerButtonEvent> Esi::HumanInterfaceDevices::getControllerButtonEvents()
{
	return this->controllerButtonEvents;
}

std::unordered_map<SDL_Keycode, SDL_KeyboardEvent> Esi::HumanInterfaceDevices::getKeyboardEvents()
{
	return this->keyboardEvents;
}

std::unordered_map<Esi::HumanInterfaceDevices::ControllerAxisCode, SDL_ControllerAxisEvent> Esi::HumanInterfaceDevices::getControllerAxisEvents()
{
	return this->controllerAxisEvents;
}

SDL_MouseButtonEvent Esi::HumanInterfaceDevices::getMouseButtonEvent()
{
	return this->mouseButtonEvent;
}

SDL_MouseMotionEvent Esi::HumanInterfaceDevices::getMouseMotionEvent()
{
	return this->mouseMotionEvent;
}

SDL_MouseWheelEvent  Esi::HumanInterfaceDevices::getMouseWheelEvent()
{
	return this->mouseWheelEvent;
}

std::unordered_map<Esi::PlayerID, SDL_JoystickID> Esi::HumanInterfaceDevices::getControllerPlayerMap()
{
	return this->controllerPlayerMap;
}

std::queue<Esi::PlayerID>& Esi::HumanInterfaceDevices::getNewlyLoadedPlayerIDs()
{
	return this->newlyLoadedPlayerIDs;
}