#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Collections/Vector.hpp"
#include "../../../Utilities/Constants.hpp"
#include <stdint.h>
#include "Component.hpp"

namespace Lilliputian
{
	class PropertyAnimation : public Component
	{
	public:
		float* targetDoubleProperty = NULL;
		Vector<float> keyFrames;
		float duration_s = 0.0;
		float playbackSpeed_pct = 1.0;
		float delay_s = 0.0;
		float progress_s = 0.0;
		float maximumValue = 0.0;
		float minimumValue = 0.0;
		uint8_t repeats = 0;
		uint8_t repeatCount = 0;
		bool isPlaying = false;
	private:
	};
}