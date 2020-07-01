#pragma once

#include <SDL.h>
#include "../../Game/Scene.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "RenderingSDL/SDLRenderable.hpp"

namespace Lilliputian
{
	class RenderingEngine
	{
	private:
		SDL_Renderer* sdlRenderer;
		SDL_Window* sdlWindow;
		Vector<SDLRenderable> sdlRenderables;

		void clearBuffers();
		void render();
	public:
		RenderingEngine(SDL_Window* sdlWindow);
		void process(Scene* scene);
	};
}

