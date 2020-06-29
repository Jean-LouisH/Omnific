#include "HapticEngine.hpp"
#include <SDL.h>

Lilliputian::HapticEngine::HapticEngine()
{
	hapticRequests.reserve(8);
}

void Lilliputian::HapticEngine::process(
	Scene* scene,
	Vector<SDL_Haptic*> haptics)
{
	//Todo: Populate requests from scene

	for (int i = 0; i < this->hapticRequests.size(); i++)
	{
		HapticRequest currentHapticRequest = this->hapticRequests.at(i);

		SDL_HapticRumblePlay(
			haptics.at(currentHapticRequest.controllerID),
			currentHapticRequest.strength_pct, 
			currentHapticRequest.duration_ms);
	}

	hapticRequests.clear();
}