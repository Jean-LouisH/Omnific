#include "asset_cache.hpp"

Lilliputian::StreamedAudio Lilliputian::AssetCache::loadStreamedAudio(const char* filepath)
{
	if (!this->streamedAudios.count(filepath))
	{
		this->streamedAudios.emplace(filepath, StreamedAudio(filepath));
	}

	return this->streamedAudios.at(filepath);
}

Lilliputian::Font Lilliputian::AssetCache::loadFont(const char* filepath)
{
	if (!this->fonts.count(filepath))
	{
		this->fonts.emplace(filepath, Font(filepath));
	}

	return this->fonts.at(filepath);
}

Lilliputian::Text Lilliputian::AssetCache::loadText(const char* filepath)
{
	if (!this->texts.count(filepath))
	{
		this->texts.emplace(filepath, Text(filepath));
	}

	return this->texts.at(filepath);
}

Lilliputian::Texture Lilliputian::AssetCache::loadTexture(Image* image)
{
	if (!this->textures.count(image))
	{
		this->textures.emplace(image, Texture(image));
	}

	return this->textures.at(image);
}

void Lilliputian::AssetCache::deleteStreamedAudio(const char* filepath)
{
	this->streamedAudios.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteFont(const char* filepath)
{
	this->fonts.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteText(const char* filepath)
{
	this->texts.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteTexture(Image* image)
{
	this->textures.at(image).unload();
}

void Lilliputian::AssetCache::deleteAllAudio()
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

void Lilliputian::AssetCache::deleteAllFonts()
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

void Lilliputian::AssetCache::deleteAllTexts()
{
	if (!this->texts.empty())
	{
		for (std::pair<String, Text> element : this->texts)
		{
			element.second.unload();
		}
		this->texts.clear();
	}
}

void Lilliputian::AssetCache::deleteAllTextures()
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

void Lilliputian::AssetCache::deleteAll()
{
	deleteAllFonts();
	deleteAllAudio();
	deleteAllTextures();
}

Lilliputian::Map<Lilliputian::String, Lilliputian::StreamedAudio> Lilliputian::AssetCache::getStreamedAudios()
{
	return this->streamedAudios;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Font> Lilliputian::AssetCache::getFonts()
{
	return this->fonts;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Text> Lilliputian::AssetCache::getTexts()
{
	return this->texts;
}

Lilliputian::Map<Lilliputian::Image*, Lilliputian::Texture> Lilliputian::AssetCache::getTextures()
{
	return this->textures;
}