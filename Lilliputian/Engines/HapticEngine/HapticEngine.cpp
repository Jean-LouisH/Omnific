#include "HapticEngine.hpp"
#include <SDL.h>

Lilliputian::HapticEngine::HapticEngine()
{
	hapticRequests.reserve(8);
}

void Lilliputian::HapticEngine::rumble(Vector<SDL_Haptic*> haptics)
{
	for (int i = 0; i < this->hapticRequests.size(); i++)
	{
		HapticRequest currentHapticRequest = this->hapticRequests.at(i);

		SDL_HapticRumblePlay(
			haptics.at(currentHapticRequest.controllerID),
			currentHapticRequest.strength_pct,
			currentHapticRequest.duration_ms);
	}
}

void Lilliputian::HapticEngine::process(
	Scene& scene,
	HumanInterfaceDevices& hid)
{
	//Todo: Populate requests from scene

	this->rumble(hid.getHaptics());
	hapticRequests.clear();
}