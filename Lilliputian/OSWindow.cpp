#include "OSWindow.hpp"
#include <string>

Lilliputian::OSWindow::OSWindow(const char* title, uint16_t width, uint16_t height, bool isFullscreen)
{
	this->shutdownRequest = false;
	this->isFullscreen = isFullscreen;

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
				SDL_HapticRumbleInit(this->haptics.back());
				if (!this->gameControllers.back())
					fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

void Lilliputian::OSWindow::pollInputEvents()
{
	SDL_Event SDLEvents;
	
	this->inputs.clear();

	while (SDL_PollEvent(&SDLEvents))
	{
		switch (SDLEvents.type)
		{
		case SDL_QUIT:
			this->shutdownRequest = true;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			this->inputs.keyboardEvents.emplace(SDLEvents.key.keysym.sym, SDLEvents.key);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			this->inputs.mouseButtonEvent = SDLEvents.button;
			break;

		case SDL_MOUSEMOTION:
			this->inputs.mouseMotionEvent = SDLEvents.motion;
			break;

		case SDL_MOUSEWHEEL:
			this->inputs.mouseWheelEvent = SDLEvents.wheel;
			break;

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			this->inputs.controllerButtonEvents.emplace(SDLEvents.cbutton.button, SDLEvents.cbutton);
			break;

		case SDL_CONTROLLERAXISMOTION:
			this->inputs.controllerAxisEvents.emplace(SDLEvents.caxis.axis, SDLEvents.caxis);
			break;
		}
	}
}

void Lilliputian::OSWindow::setToWindowed(uint16_t width_px, uint16_t height_px)
{
	SDL_SetWindowFullscreen(this->sdlWindow, 0);
	SDL_SetWindowSize(this->sdlWindow, width_px, height_px);

}

void Lilliputian::OSWindow::setToFullscreen(SDL_DisplayMode* mode)
{
	SDL_SetWindowDisplayMode(this->sdlWindow, mode);
	SDL_SetWindowFullscreen(this->sdlWindow, SDL_WINDOW_FULLSCREEN);
}

void Lilliputian::OSWindow::toggleWindowedFullscreen()
{
	this->isFullscreen = !isFullscreen;

	if (this->isFullscreen)
	{
		SDL_SetWindowFullscreen(this->sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(this->sdlWindow, 0);
		SDL_SetWindowPosition(
			this->sdlWindow,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED);
	}

	SDL_ShowCursor(!this->isFullscreen);
}

bool Lilliputian::OSWindow::hasRequestedShutdown()
{
	return this->shutdownRequest;
}

SDL_Window* Lilliputian::OSWindow::getSDLWindow()
{
	return this->sdlWindow;
}

Lilliputian::OSWindowInputs Lilliputian::OSWindow::getInputs()
{
	return this->inputs;
}

Lilliputian::Vector<SDL_Haptic*> Lilliputian::OSWindow::getHaptics()
{
	return this->haptics;
}