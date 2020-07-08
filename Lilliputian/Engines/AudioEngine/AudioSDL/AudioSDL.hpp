#pragma once

#include <SDL_mixer.h>
#include <queue>

namespace SDL
{
	namespace Audio
	{
		void playSounds(
			std::queue<Mix_Chunk*>* immediateSounds,
			std::queue<Mix_Chunk*>* scheduledSounds);
	}
}