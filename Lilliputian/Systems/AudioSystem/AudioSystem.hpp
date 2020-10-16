#pragma once

#include <SDL_mixer.h>
#include "../../Utilities/Collections/Queue.hpp"
#include "../../Game/Scene/Scene.hpp"

namespace Lilliputian
{
	class AudioSystem
	{
	private:
		Queue<Mix_Chunk*> immediateSounds;
		Queue<Mix_Chunk*> scheduledSounds;

		void play();
	public:
		AudioSystem();
		~AudioSystem();
		void process(Scene& scene);
	};
}

