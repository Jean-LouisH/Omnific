#include "AudioSDL.hpp"

void Audio::SDL::playSounds(
	std::queue<Mix_Chunk*>* immediateSounds,
	std::queue<Mix_Chunk*>* scheduledSounds)
{
	while (!immediateSounds->empty())
	{
		Mix_PlayChannel(-1, immediateSounds->front(), 0);
		immediateSounds->pop();
	}

	if (!scheduledSounds->empty())
	{
		if (!Mix_Playing(-1))
		{
			Mix_PlayChannel(-1, scheduledSounds->front(), 0);
			scheduledSounds->pop();
		}
	}
}