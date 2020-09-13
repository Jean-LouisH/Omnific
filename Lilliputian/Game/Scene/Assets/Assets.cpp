#include "Assets.hpp"

Lilliputian::StreamedAudio Lilliputian::Assets::loadStreamedAudio(const char* filepath)
{
	if (!this->streamedAudios.count(filepath))
	{
		this->streamedAudios.emplace(filepath, StreamedAudio(filepath));
	}

	return this->streamedAudios.at(filepath);
}

Lilliputian::Font Lilliputian::Assets::loadFont(const char* filepath)
{
	if (!this->fonts.count(filepath))
	{
		this->fonts.emplace(filepath, Font(filepath));
	}

	return this->fonts.at(filepath);
}

Lilliputian::Texture Lilliputian::Assets::loadTexture(Image* image)
{
	if (!this->textures.count(image))
	{
		this->textures.emplace(image, Texture(image));
	}

	return this->textures.at(image);
}

void Lilliputian::Assets::deleteStreamedAudio(const char* filepath)
{
	this->streamedAudios.at(filepath).unload();
}

void Lilliputian::Assets::deleteFont(const char* filepath)
{
	this->fonts.at(filepath).unload();
}

void Lilliputian::Assets::deleteTexture(Image* image)
{
	this->textures.at(image).unload();
}

void Lilliputian::Assets::deleteAllAudio()
{
	if (!this->streamedAudios.empty())
	{
		for (std::pair<String, StreamedAudio> element : this->streamedAudios)
		{
			element.second.unload();
		}
		this->streamedAudios.clear();
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
		for (std::pair<Image*, Texture> element : this->textures)
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

Lilliputian::Map<Lilliputian::String, Lilliputian::StreamedAudio> Lilliputian::Assets::getStreamedAudios()
{
	return this->streamedAudios;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Font> Lilliputian::Assets::getFonts()
{
	return this->fonts;
}

Lilliputian::Map<Lilliputian::Image*, Lilliputian::Texture> Lilliputian::Assets::getTextures()
{
	return this->textures;
}