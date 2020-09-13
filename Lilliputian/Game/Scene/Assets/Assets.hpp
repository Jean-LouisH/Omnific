#include "../../../Utilities/Collections/Map.hpp"
#include "../../../Utilities/String.hpp"
#include "../../../Utilities/Image.hpp"
#include "StreamedAudio.hpp"
#include "Font.hpp"
#include "Texture.hpp"

namespace Lilliputian
{
	class Assets
	{
	private:
		Map<String, StreamedAudio> streamedAudios;
		Map<String, Font> fonts;
		Map<Image*, Texture> textures;
	public:
		StreamedAudio loadStreamedAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Texture loadTexture(Image* image);

		void deleteStreamedAudio(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteTexture(Image* image);

		void deleteAllAudio();
		void deleteAllFonts();
		void deleteAllTextures();

		void deleteAll();

		Map<String, StreamedAudio> getStreamedAudios();
		Map<String, Font> getFonts();
		Map<Image*, Texture> getTextures();
	};
}