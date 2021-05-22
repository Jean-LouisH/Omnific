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

#include "texture_cache.hpp"

bool Lilliputian::TextureCache::containsKey(AssetID assetID)
{
	return this->sdlTextureCache.count(assetID) > 0;
}

void Lilliputian::TextureCache::emplace(AssetID assetID, SDL_Texture* sdlTexture) 
{
	this->sdlTextureCache.emplace(assetID, sdlTexture);
	this->missedFrameCounts.emplace(assetID, 0);
}

SDL_Texture* Lilliputian::TextureCache::at(AssetID assetID)
{
	SDL_Texture* texture = nullptr;

	if (this->containsKey(assetID))
	{
		texture = this->sdlTextureCache.at(assetID);
		this->missedFrameCounts.at(assetID) = 0;
	}

	return texture;
}

void Lilliputian::TextureCache::collectGarbage()
{
	for (auto it = this->missedFrameCounts.cbegin(); it != this->missedFrameCounts.cend();)
	{
		if (this->missedFrameCounts.at(it->first) > this->allowableMissedFrames)
		{
			SDL_DestroyTexture(this->sdlTextureCache.at(it->first));
			this->sdlTextureCache.erase(it->first);
			this->missedFrameCounts.erase(it++);
		}
		else
		{
			++it;
		}
	}

	for (auto it = this->missedFrameCounts.begin(); it != this->missedFrameCounts.end(); ++it)
	{
		this->missedFrameCounts.at(it->first)++;
	}
}

void Lilliputian::TextureCache::setAllowableMissedFrames(unsigned int missedFrameCount)
{
	this->allowableMissedFrames = missedFrameCount;
}