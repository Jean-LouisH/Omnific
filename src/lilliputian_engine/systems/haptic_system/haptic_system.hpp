#pragma once

#include "game/scene/scene_forest.hpp"
#include <SDL.h>
#include "utilities/collections/vector.hpp"
#include "human_interface_devices.hpp"

namespace Lilliputian
{
	class HapticSystem
	{
	public:
		HapticSystem();
		void process(SceneForest& scene, HumanInterfaceDevices& hid);
	private:
		typedef struct HapticRequest
		{
			uint8_t controllerID;
			float strength_pct;
			uint16_t duration_ms;
		}HapticRequest;

		Vector<HapticRequest> hapticRequests;

		void rumble(Vector<SDL_Haptic*> haptics);
	};
}

