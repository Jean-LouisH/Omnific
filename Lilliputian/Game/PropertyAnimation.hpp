#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include "../Utilities/Constants.hpp"
#include <stdint.h>

namespace Lilliputian
{
	class PropertyAnimation
	{
	private:
	public:
		EntityID entityID = NO_ENTITY;

		double* targetDoubleProperty = NULL;
		Vector<double> keyFrames;
		double duration_s = 0.0;
		double playbackSpeed_pct = 1.0;
		double delay_s = 0.0;
		double progress_s = 0.0;
		double maximumValue = 0.0;
		double minimumValue = 0.0;
		uint8_t repeats = 0;
		uint8_t repeatCount = 0;
		bool isPlaying = false;
	};
}