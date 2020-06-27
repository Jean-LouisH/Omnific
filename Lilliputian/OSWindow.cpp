#include "OSWindow.hpp"
#include <string>

Lilliputian::OSWindow::OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen)
{
	this->shutdownRequest = false;
	this->sdlWindow = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN_DESKTOP & isFullscreen);
}

void Lilliputian::OSWindow::sleep(int time_ms)
{
	if (time_ms > 0)
		SDL_Delay(time_ms);
}

void Lilliputian::OSWindow::detectGameControllers()
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
				if (!this->gameControllers.back())
					fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

void Lilliputian::OSWindow::pollInputEvents()
{
	SDL_Event SDLEvents;

	this->releasedKeys.clear();

	while (SDL_PollEvent(&SDLEvents))
	{
		switch (SDLEvents.type)
		{
			/*Close window button*/
		case SDL_QUIT:
			this->shutdownRequest = true;
			break;

			/*Keyboard inputs*/
		case SDL_KEYDOWN:
			KeyEvent press;
			press.mod = SDLEvents.key.keysym.mod;
			press.timestamp_ms = SDLEvents.key.timestamp;
			this->pressedKeys.emplace(SDLEvents.key.keysym.sym, press);
			break;
		case SDL_KEYUP:
			KeyEvent release;
			release.mod = SDLEvents.key.keysym.mod;
			release.timestamp_ms = SDLEvents.key.timestamp;
			this->releasedKeys.emplace(SDLEvents.key.keysym.sym, release);
			this->pressedKeys.erase(SDLEvents.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			this->mouse.type = SDLEvents.type;
			this->mouse.timestamp_ms = SDLEvents.button.timestamp;
			this->mouse.windowID = SDLEvents.button.windowID;
			this->mouse.button = SDLEvents.button.button;
			this->mouse.buttonState = SDLEvents.button.state;
			this->mouse.clicks = SDLEvents.button.clicks;
			this->mouse.x = SDLEvents.button.x;
			this->mouse.y = SDLEvents.button.y;
			break;
		case SDL_MOUSEMOTION:
			this->mouse.type = SDL_MOUSEMOTION;
			this->mouse.timestamp_ms = SDLEvents.motion.timestamp;
			this->mouse.windowID = SDLEvents.motion.windowID;
			this->mouse.motionState = SDLEvents.motion.state;
			this->mouse.x = SDLEvents.motion.x;
			this->mouse.y = SDLEvents.motion.y;
			this->mouse.xrel = SDLEvents.motion.xrel;
			this->mouse.yrel = SDLEvents.motion.yrel;
			break;
		case SDL_MOUSEWHEEL:
			this->mouse.type = SDL_MOUSEWHEEL;
			this->mouse.timestamp_ms = SDLEvents.wheel.timestamp;
			this->mouse.windowID = SDLEvents.wheel.windowID;
			this->mouse.xScroll = SDLEvents.wheel.x;
			this->mouse.yScroll = SDLEvents.wheel.y;
			this->mouse.direction = SDLEvents.wheel.direction;
			break;
		}
	}
}

bool Lilliputian::OSWindow::hasRequestedShutdown()
{
	return this->shutdownRequest;
}

SDL_Window* Lilliputian::OSWindow::getSDLWindow()
{
	return this->sdlWindow;
}