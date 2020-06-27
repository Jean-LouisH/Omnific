#pragma once

#include <SDL.h>
#include "../../Game/Scene.hpp"

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
		void render(Scene* scene);
	};
}

