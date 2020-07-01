#pragma once

#include <SDL_mixer.h>
#include "../../Utilities/Collections/Queue.hpp"
#include "../../Game/Scene.hpp"

namespace Lilliputian
{
	class AudioEngine
	{
	private:
		Queue<Mix_Chunk*> immediateSounds;
		Queue<Mix_Chunk*> scheduledSounds;

		void play();
	public:
		AudioEngine();
		void process(Scene* scene);
	};
}

