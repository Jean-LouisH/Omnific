#pragma once

#include "../../Game/Scene/Scene.hpp"
#include <SDL.h>
#include "../../Utilities/Collections/Vector.hpp"
#include "../../HumanInterfaceDevices.hpp"

namespace Lilliputian
{
	class HapticSystem
	{
	public:
		HapticSystem();
		void process(Scene& scene, HumanInterfaceDevices& hid);
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

