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

#pragma once

#include "utilities/collections/map.hpp"
#include "utilities/string.hpp"
#include "audio_stream.hpp"
#include "font.hpp"
#include "text.hpp"
#include "image.hpp"

namespace Lilliputian
{
	class AssetCache
	{
	public:
		AudioStream loadStreamedAudio(const char* filepath);
		Font loadFont(const char* filepath);
		Text loadText(const char* filepath);
		Image loadTexture(const char* filepath);

		void deleteStreamedAudio(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteText(const char* filepath);
		void deleteTexture(const char* filepath);

		void deleteAllAudio();
		void deleteAllFonts();
		void deleteAllTexts();
		void deleteAllTextures();

		void deleteAll();

		Map<String, AudioStream> getStreamedAudios();
		Map<String, Font> getFonts();
		Map<String, Text> getTexts();
		Map<String, Image> getTextures();
	private:
		Map<String, AudioStream> streamedAudios;
		Map<String, Font> fonts;
		Map<String, Text> texts;
		Map<String, Image> textures;
	};
}