#pragma once

#include <SDL.h>
#include "../../Game/Scene/Scene.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "RenderingSDL/SDLRenderable.hpp"
#include "../../Window.hpp"

namespace Lilliputian
{
	class RenderingEngine
	{
	private:
		SDL_Renderer* sdlRenderer;
		SDL_Window* sdlWindow;
		Vector<SDL::Rendering2D::SDLRenderable> sdlRenderables;

		void clearBuffers();
		void render();
	public:
		RenderingEngine(Window& window);
		void process(Scene& scene);
	};
}

