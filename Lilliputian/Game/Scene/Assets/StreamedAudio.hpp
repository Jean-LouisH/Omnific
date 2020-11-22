#pragma once

#include "../../../Utilities/Aliases.hpp"
#include <SDL_mixer.h>
#include "Asset.hpp"

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