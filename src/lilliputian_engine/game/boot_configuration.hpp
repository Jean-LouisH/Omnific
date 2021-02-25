#pragma once

#include "utilities/string.hpp"

namespace Lilliputian 
{
	class BootConfiguration
	{
	public:
		//Game
		String gameTitle;
		String version;
		String developer;
		String iconFilepath;
		String entryScenePath;
		//Game Window
		uint16_t windowHeight;
		uint16_t windowWidth;
		bool isStartingFullscreen;
		bool isStartingMaximized;
		bool isResizable;
		//Time
		uint32_t msPerComputeUpdate;
		uint32_t targetFPS;
	private:
	};
}