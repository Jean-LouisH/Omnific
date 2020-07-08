#pragma once

#include "../Utilities/Aliases.hpp"
#include <SDL_mixer.h>

namespace Lilliputian
{
	class Audio
	{
	private:
		Mix_Chunk* sound;
	public:
		Audio(const char* filepath);
		void unload();
	};
}