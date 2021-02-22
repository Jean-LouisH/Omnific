#pragma once

#include "../../../utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class CountdownTimer : public Component
	{
	public:
		void start(float time);
		void update(float deltaTime);
		void stop();
		bool isFinished();
	private:
		bool isActive = false;
		float startTime = 0.0;
		float currentTime = 0.0;
	};
}