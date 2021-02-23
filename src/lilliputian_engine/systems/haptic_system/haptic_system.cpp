#include "haptic_system.hpp"
#include <SDL.h>

Lilliputian::HapticSystem::HapticSystem()
{
	hapticRequests.reserve(8);
}

void Lilliputian::HapticSystem::rumble(Vector<SDL_Haptic*> haptics)
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

void Lilliputian::HapticSystem::process(
	Scene& scene,
	HumanInterfaceDevices& hid)
{
	//Todo: Populate requests from scene

	this->rumble(hid.getHaptics());
	hapticRequests.clear();
}