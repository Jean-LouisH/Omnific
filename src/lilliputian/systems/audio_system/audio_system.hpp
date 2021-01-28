#pragma once

#include <SDL_mixer.h>
#include "../../utilities/collections/queue.hpp"
#include "../../game/scene/scene.hpp"

namespace Lilliputian
{
	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();
		void process(Scene& scene);
	private:
		Queue<Mix_Chunk*> immediateSounds;
		Queue<Mix_Chunk*> scheduledSounds;

		void play();
	};
}

