//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "asset_cache.hpp"

Lilliputian::AudioStream Lilliputian::AssetCache::loadStreamedAudio(const char* filepath)
{
	if (!this->streamedAudios.count(filepath))
	{
		this->streamedAudios.emplace(filepath, AudioStream(filepath));
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

Lilliputian::Image Lilliputian::AssetCache::loadTexture(const char* filepath)
{
	if (!this->textures.count(filepath))
	{
		this->textures.emplace(filepath, Image(filepath));
	}

	return this->textures.at(filepath);
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

void Lilliputian::AssetCache::deleteTexture(const char* filepath)
{
	this->textures.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteAllAudio()
{
	if (!this->streamedAudios.empty())
	{
		for (std::pair<String, AudioStream> element : this->streamedAudios)
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
		for (std::pair<String, Image> element : this->textures)
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

Lilliputian::Map<Lilliputian::String, Lilliputian::AudioStream> Lilliputian::AssetCache::getStreamedAudios()
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

Lilliputian::Map<Lilliputian::String, Lilliputian::Image> Lilliputian::AssetCache::getTextures()
{
	return this->textures;
}