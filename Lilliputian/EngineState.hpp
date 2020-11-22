#pragma once

#include <stdint.h>

namespace Lilliputian
{
	class EngineState
	{
	public:
		void setInitializing();
		void setRunningApplicationWindowed();
		void setRunningApplicationFullscreen();
		void setRunningApplicationFullscreenDesktop();
		void setRestarting();
		void setShuttingDown();

		bool isInitializing();
		bool isRunningApplicationWindowed();
		bool isRunningApplicationFullscreen();
		bool isRunningApplicationFullscreenDesktop();
		bool isRestarting();
		bool isShuttingDown();

		bool isRunning();

		EngineState();
	private:
		enum EngineStates
		{
			ENGINE_STATE_INITIALIZING,
			ENGINE_STATE_RUNNING_APPLICATION_WINDOWED,
			ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN,
			ENGINE_STATE_RUNNING_APPLICATION_FULLSCREEN_DESKTOP,
			ENGINE_STATE_RESTARTING,
			ENGINE_STATE_SHUTTING_DOWN,
		};

		uint8_t value;
	};
}