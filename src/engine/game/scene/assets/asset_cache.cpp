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

Lilliputian::AudioStream Lilliputian::AssetCache::loadAudioStream(const char* filepath)
{
	if (!this->audioStreams.count(filepath))
	{
		this->audioStreams.emplace(filepath, AudioStream(filepath));
	}

	return this->audioStreams.at(filepath);
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

Lilliputian::Image Lilliputian::AssetCache::loadImage(const char* filepath)
{
	if (!this->images.count(filepath))
	{
		this->images.emplace(filepath, Image(filepath));
	}

	return this->images.at(filepath);
}

void Lilliputian::AssetCache::deleteAudioStream(const char* filepath)
{
	this->audioStreams.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteFont(const char* filepath)
{
	this->fonts.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteText(const char* filepath)
{
	this->texts.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteImage(const char* filepath)
{
	this->images.at(filepath).unload();
}

void Lilliputian::AssetCache::deleteAllAudioStreams()
{
	if (!this->audioStreams.empty())
	{
		for (std::pair<String, AudioStream> element : this->audioStreams)
		{
			element.second.unload();
		}
		this->audioStreams.clear();
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

void Lilliputian::AssetCache::deleteAllImages()
{
	if (!this->images.empty())
	{
		for (std::pair<String, Image> element : this->images)
		{
			element.second.unload();
		}
		this->images.clear();
	}
}

void Lilliputian::AssetCache::deleteAll()
{
	deleteAllFonts();
	deleteAllAudioStreams();
	deleteAllImages();
}

Lilliputian::Map<Lilliputian::String, Lilliputian::AudioStream> Lilliputian::AssetCache::getAudioStreams()
{
	return this->audioStreams;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Font> Lilliputian::AssetCache::getFonts()
{
	return this->fonts;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Text> Lilliputian::AssetCache::getTexts()
{
	return this->texts;
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Image> Lilliputian::AssetCache::getImages()
{
	return this->images;
}