#include "RenderingEngine.hpp"

Lilliputian::RenderingEngine::RenderingEngine(SDL_Window* sdlWindow)
{
	this->sdlWindow = sdlWindow;
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
	SDL_RenderPresent(this->sdlRenderer);
}