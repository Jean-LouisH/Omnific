#pragma once

#include "utilities/collections/map.hpp"
#include "utilities/string.hpp"
#include "utilities/image.hpp"
#include "audio_stream.hpp"
#include "font.hpp"
#include "text.hpp"
#include "texture.hpp"

namespace Lilliputian
{
	class AssetCache
	{
	public:
		AudioStream loadStreamedAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Text loadText(const char* filepath);
		Texture loadTexture(Image image);

		void deleteStreamedAudio(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteText(const char* filepath);
		void deleteTexture(Image image);

		void deleteAllAudio();
		void deleteAllFonts();
		void deleteAllTexts();
		void deleteAllTextures();

		void deleteAll();

		Map<String, AudioStream> getStreamedAudios();
		Map<String, Font> getFonts();
		Map<String, Text> getTexts();
		Map<ImageHash, Texture> getTextures();
	private:
		Map<String, AudioStream> streamedAudios;
		Map<String, Font> fonts;
		Map<String, Text> texts;
		Map<ImageHash, Texture> textures;
	};
}