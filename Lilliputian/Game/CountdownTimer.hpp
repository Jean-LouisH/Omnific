#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Constants.hpp"

namespace Lilliputian
{
	class CountdownTimer
	{
	private:
		bool isActive = false;
		double startTime = 0.0;
		double currentTime = 0.0;
	public:
		EntityID entityID = NO_ENTITY;

		void start(double time);
		void update(double deltaTime);
		void stop();
		bool isFinished();
	};
}