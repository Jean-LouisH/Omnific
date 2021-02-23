#include "human_interface_devices.hpp"

void Lilliputian::HumanInterfaceDevices::clear()
{
	this->controllerAxisEvents.clear();
	this->controllerButtonEvents.clear();
	this->keyboardEvents.clear();

	if (dropEvent.file != NULL)
	{
		SDL_free(dropEvent.file);
		dropEvent = { 0 };
	}
}

void Lilliputian::HumanInterfaceDevices::detectGameControllers()
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

		for (int i = 0; i < SDL_NumJoysticks(); ++i)
		{
			if (SDL_IsGameController(i))
			{
				SDL_GameController* newController = SDL_GameControllerOpen(i);
				SDL_Joystick* joystick = SDL_GameControllerGetJoystick(newController);
				this->gameControllers.push_back(newController);
				this->haptics.push_back(SDL_HapticOpenFromJoystick(joystick));
				SDL_HapticRumbleInit(this->haptics.back());
				if (!this->gameControllers.back())
					fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

void Lilliputian::HumanInterfaceDevices::pollInputEvents()
{
	SDL_Event SDLEvents;

	this->clear();

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
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			this->mouseButtonEvent = SDLEvents.button;
			break;

		case SDL_MOUSEMOTION:
			this->mouseMotionEvent = SDLEvents.motion;
			break;

		case SDL_MOUSEWHEEL:
			this->mouseWheelEvent = SDLEvents.wheel;
			break;

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			this->controllerButtonEvents.emplace(SDLEvents.cbutton.button, SDLEvents.cbutton);
			break;

		case SDL_CONTROLLERAXISMOTION:
			this->controllerAxisEvents.emplace(SDLEvents.caxis.axis, SDLEvents.caxis);
			break;

		case SDL_DROPFILE:
			this->dropEvent = SDLEvents.drop;
		}
	}
}

bool Lilliputian::HumanInterfaceDevices::hasRequestedShutdown()
{
	return this->shutdownRequest;
}

Lilliputian::Vector<SDL_Haptic*> Lilliputian::HumanInterfaceDevices::getHaptics()
{
	return this->haptics;
}