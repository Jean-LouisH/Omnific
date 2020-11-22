#pragma once

#include "../../../Utilities/Aliases.hpp"
#include <SDL_mixer.h>
#include "Asset.hpp"

namespace Lilliputian
{
	class StreamedAudio : public Asset
	{
	private:
		Mix_Chunk* sound;
	public:
		StreamedAudio(const char* filepath);
	};
}