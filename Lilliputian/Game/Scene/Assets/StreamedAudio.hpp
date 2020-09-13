#pragma once

#include "../../../Utilities/Aliases.hpp"
#include <SDL_mixer.h>

namespace Lilliputian
{
	class StreamedAudio
	{
	private:
		Mix_Chunk* sound;
	public:
		StreamedAudio(const char* filepath);
		void unload();
	};
}