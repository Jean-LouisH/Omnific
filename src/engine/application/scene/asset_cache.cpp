// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "asset_cache.hpp"

Esi::AudioStream Esi::AssetCache::loadAudioStream(const char* filepath)
{
	if (!this->audioStreams.count(filepath))
	{
		this->audioStreams.emplace(filepath, AudioStream(filepath));
	}

	return this->audioStreams.at(filepath);
}

Esi::Font Esi::AssetCache::loadFont(const char* filepath, uint16_t size_px)
{
	if (!this->fonts.count(filepath))
	{
		this->fonts.emplace(filepath, Font(filepath, size_px));
	}

	return this->fonts.at(filepath);
}

Esi::Text Esi::AssetCache::loadText(const char* filepath)
{
	if (!this->texts.count(filepath))
	{
		this->texts.emplace(filepath, Text(filepath));
	}

	return this->texts.at(filepath);
}

Esi::Image Esi::AssetCache::loadImage(const char* filepath)
{
	if (!this->images.count(filepath))
	{
		this->images.emplace(filepath, Image(filepath));
	}

	return this->images.at(filepath);
}

void Esi::AssetCache::deleteAudioStream(const char* filepath)
{
	this->audioStreams.at(filepath).unload();
}

void Esi::AssetCache::deleteFont(const char* filepath)
{
	this->fonts.at(filepath).unload();
}

void Esi::AssetCache::deleteText(const char* filepath)
{
	this->texts.at(filepath).unload();
}

void Esi::AssetCache::deleteImage(const char* filepath)
{
	this->images.at(filepath).unload();
}

void Esi::AssetCache::deleteAllAudioStreams()
{
	if (!this->audioStreams.empty())
	{
		for (std::pair<std::string, AudioStream> element : this->audioStreams)
		{
			element.second.unload();
		}
		this->audioStreams.clear();
	}
}

void Esi::AssetCache::deleteAllFonts()
{
	if (!this->fonts.empty())
	{
		for (std::pair<std::string, Font> element : this->fonts)
		{
			element.second.unload();
		}
		this->fonts.clear();
	}
}

void Esi::AssetCache::deleteAllTexts()
{
	if (!this->texts.empty())
	{
		for (std::pair<std::string, Text> element : this->texts)
		{
			element.second.unload();
		}
		this->texts.clear();
	}
}

void Esi::AssetCache::deleteAllImages()
{
	if (!this->images.empty())
	{
		for (std::pair<std::string, Image> element : this->images)
		{
			element.second.unload();
		}
		this->images.clear();
	}
}

void Esi::AssetCache::deleteAll()
{
	deleteAllFonts();
	deleteAllAudioStreams();
	deleteAllImages();
}

std::unordered_map<std::string, Esi::AudioStream> Esi::AssetCache::getAudioStreams()
{
	return this->audioStreams;
}

std::unordered_map<std::string, Esi::Font> Esi::AssetCache::getFonts()
{
	return this->fonts;
}

std::unordered_map<std::string, Esi::Text> Esi::AssetCache::getTexts()
{
	return this->texts;
}

std::unordered_map<std::string, Esi::Image> Esi::AssetCache::getImages()
{
	return this->images;
}