#include "RenderingEngine.hpp"
#include "RenderingSDL/RenderingSDL.hpp"
#include <SDL_image.h>

Lilliputian::RenderingEngine::RenderingEngine(Window& window)
{
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	this->sdlWindow = window.getSDLWindow();
	this->sdlRenderer = SDL_CreateRenderer(
		sdlWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Lilliputian::RenderingEngine::clearBuffers()
{
	SDL_SetRenderDrawColor(this->sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(this->sdlRenderer);
}

void Lilliputian::RenderingEngine::render()
{
	for (int i = 0; i < this->sdlRenderables.size(); i++)
	{	
		SDL::Rendering2D::SDLRenderable sdlRenderable = this->sdlRenderables.at(i);
		SDL_RenderCopyEx(
			this->sdlRenderer,
			sdlRenderable.texture,
			&sdlRenderable.textureRect,
			&sdlRenderable.renderingRect,
			sdlRenderable.rotation,
			NULL,
			sdlRenderable.flip
		);
	}
	SDL_RenderPresent(this->sdlRenderer);
}

void Lilliputian::RenderingEngine::process(Scene& scene)
{
	Vector<SDL::Rendering2D::Sprite2D> sprite2Ds;
	SDL::Rendering2D::Camera2D camera2D;

	//todo: build renderables from scene.

	SDL::Rendering2D::buildRenderablesFromSprites(
		&this->sdlRenderables,
		sprite2Ds,
		camera2D,
		this->sdlWindow);

	this->clearBuffers();
	this->render();
}