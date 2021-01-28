#pragma once

#include <SDL.h>
#include "../../game/scene/scene.hpp"
#include "../../utilities/collections/vector.hpp"
#include "rendering_sdl/sdl_renderable.hpp"
#include "../../window.hpp"

namespace Lilliputian
{
	class RenderingSystem
	{
	public:
		RenderingSystem(Window& window);
		void process(Scene& scene);
	private:
		SDL_Renderer* sdlRenderer;
		SDL_Window* sdlWindow;
		Vector<SDL::Rendering2D::SDLRenderable> sdlRenderables;

		void clearBuffers();
		void render();
	};
}

