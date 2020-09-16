#pragma once

namespace Lilliputian 
{
	class EngineConfiguration
	{
	private:
	public:
		uint32_t msPerComputeUpdate;
		uint32_t targetFPS;
		String gameTitle;
		uint16_t windowHeight;
		uint16_t windowWidth;
		String entryScenePath;
		bool isStartingFullscreen;
	};
}