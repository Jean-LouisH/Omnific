#pragma once

#include <SDL_mixer.h>
#include "utilities/collections/queue.hpp"
#include "game/scene/scene_forest.hpp"

namespace Lilliputian
{
	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();
		void process(SceneForest& scene);
	private:
		Queue<Mix_Chunk*> immediateSounds;
		Queue<Mix_Chunk*> scheduledSounds;

		void play();
	};
}

