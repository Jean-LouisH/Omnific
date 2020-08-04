#include "Assets.hpp"

Lilliputian::Audio Lilliputian::Assets::loadAudio(const char* filepath)
{
	if (!this->audios.count(filepath))
	{
		this->audios.emplace(filepath, Audio(filepath));
	}

	return this->audios.at(filepath);
}

Lilliputian::Font Lilliputian::Assets::loadFont(const char* filepath)
{
	if (!this->fonts.count(filepath))
	{
		this->fonts.emplace(filepath, Font(filepath));
	}

	return this->fonts.at(filepath);
}

Lilliputian::Texture Lilliputian::Assets::loadTexture(const char* filepath)
{
	if (!this->textures.count(filepath))
	{
		this->textures.emplace(filepath, Texture(filepath));
	}

	return this->textures.at(filepath);
}

void Lilliputian::Assets::deleteAudio(const char* filepath)
{
	this->audios.at(filepath).unload();
}

void Lilliputian::Assets::deleteFont(const char* filepath)
{
	this->fonts.at(filepath).unload();
}

void Lilliputian::Assets::deleteTexture(const char* filepath)
{
	this->textures.at(filepath).unload();
}

void Lilliputian::Assets::deleteAllAudio()
{
	if (!this->audios.empty())
	{
		for (std::pair<String, Audio> element : this->audios)
		{
			element.second.unload();
		}
		this->audios.clear();
	}
}

void Lilliputian::Assets::deleteAllFonts()
{
	if (!this->fonts.empty())
	{
		for (std::pair<String, Font> element : this->fonts)
		{
			element.second.unload();
		}
		this->fonts.clear();
	}
}

void Lilliputian::Assets::deleteAllTextures()
{
	if (!this->textures.empty())
	{
		for (std::pair<String, Texture> element : this->textures)
		{
			element.second.unload();
		}
		this->textures.clear();
	}
}

void Lilliputian::Assets::deleteAll()
{
	deleteAllFonts();
	deleteAllAudio();
	deleteAllTextures();
}

Lilliputian::Map<String, Audio> Lilliputian::Assets::getAudios()
{
	return this->audios;
}

Lilliputian::Map<String, Font> Lilliputian::Assets::getFonts()
{
	return this->fonts;
}

Lilliputian::Map<String, Texture> Lilliputian::Assets::getTextures()
{
	return this->textures;
}