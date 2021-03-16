#pragma once

#include "utilities/aliases.hpp"
#include <SDL_mixer.h>
#include "asset.hpp"

namespace Lilliputian
{
	class AudioStream : public Asset
	{
	public:
		AudioStream();
		AudioStream(const char* filepath);
		Mix_Chunk* getSDLMixChunk();
		virtual void unload();
	private:
		Mix_Chunk* sound;
	};
}