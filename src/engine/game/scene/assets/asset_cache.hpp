#pragma once

#include "utilities/collections/map.hpp"
#include "utilities/string.hpp"
#include "audio_stream.hpp"
#include "font.hpp"
#include "text.hpp"
#include "image.hpp"

namespace Lilliputian
{
	class AssetCache
	{
	public:
		AudioStream loadStreamedAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Text loadText(const char* filepath);
		Image loadTexture(const char* filepath);

		void deleteStreamedAudio(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteText(const char* filepath);
		void deleteTexture(const char* filepath);

		void deleteAllAudio();
		void deleteAllFonts();
		void deleteAllTexts();
		void deleteAllTextures();

		void deleteAll();

		Map<String, AudioStream> getStreamedAudios();
		Map<String, Font> getFonts();
		Map<String, Text> getTexts();
		Map<String, Image> getTextures();
	private:
		Map<String, AudioStream> streamedAudios;
		Map<String, Font> fonts;
		Map<String, Text> texts;
		Map<String, Image> textures;
	};
}