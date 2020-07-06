#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Collections/Vector.hpp"
#include <stdint.h>

namespace Lilliputian
{
	class PropertyAnimation
	{
	private:
		EntityID entityID;
	public:
		double* targetDoubleProperty;
		Vector<double> keyFrames;
		double duration_s;
		double playbackSpeed_pct;
		double delay_s;
		double progress_s;
		double maximumValue;
		double minimumValue;
		uint8_t repeats;
		uint8_t repeatCount;
		bool isPlaying;

		PropertyAnimation(EntityID entityID);
		EntityID getEntityID();
	};
}