#pragma once

#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/Constants.hpp"

namespace Lilliputian
{
	class CountdownTimer
	{
	private:
		bool isActive = false;
		float startTime = 0.0;
		float currentTime = 0.0;
	public:
		EntityID entityID = NO_ENTITY;

		void start(float time);
		void update(float deltaTime);
		void stop();
		bool isFinished();
	};
}