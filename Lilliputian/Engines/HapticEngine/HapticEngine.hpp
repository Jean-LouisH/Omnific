#pragma once

#include "../../Game/Scene.hpp"
#include <SDL.h>
#include "../../Utilities/Collections/Vector.hpp"

namespace Lilliputian
{
	class HapticEngine
	{
	private:
		typedef struct HapticRequest
		{
			uint8_t controllerID;
			double strength_pct;
			uint16_t duration_ms;
		}HapticRequest;

		Vector<HapticRequest> hapticRequests;
	public:
		HapticEngine();
		void process(Scene* scene, Vector<SDL_Haptic*> haptics);
	};
}

