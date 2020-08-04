#include "../Utilities/Collections/Map.hpp"
#include "../Utilities/String.hpp"
#include "Audio.hpp"
#include "Font.hpp"
#include "Texture.hpp"

namespace Lilliputian
{
	class Assets
	{
	private:
		Map<String, Audio> audios;
		Map<String, Font> fonts;
		Map<String, Texture> textures;
	public:
		Audio loadAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Texture loadTexture(const char* filepath);

		void deleteAudio(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteTexture(const char* filepath);

		void deleteAllAudio();
		void deleteAllFonts();
		void deleteAllTextures();

		void deleteAll();

		Map<String, Audio> getAudios();
		Map<String, Font> getFonts();
		Map<String, Texture> getTextures();
	};
}