#pragma once

#include "utilities/aliases.hpp"
#include <SDL_mixer.h>
#include "asset.hpp"

namespace Lilliputian
{
	class StreamedAudio : public Asset
	{
	public:
		StreamedAudio(const char* filepath);
		virtual void unload();
	private:
		Mix_Chunk* sound;
	};
}