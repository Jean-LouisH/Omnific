#pragma once

#include <SDL.h>

namespace Lilliputian
{
	class RenderingEngine
	{
	private:
		SDL_Renderer* sdlRenderer;
		SDL_Window* sdlWindow;
	public:
		RenderingEngine(SDL_Window* sdlWindow);
		void clearBuffers();
		void render();
	};
}

