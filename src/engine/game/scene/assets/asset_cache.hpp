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
		AudioStream loadAudioStream(const char* filepath);
		Font loadFont(const char* filepath);
		Text loadText(const char* filepath);
		Image loadImage(const char* filepath);

		void deleteAudioStream(const char* filepath);
		void deleteFont(const char* filepath);
		void deleteText(const char* filepath);
		void deleteImage(const char* filepath);

		void deleteAllAudioStreams();
		void deleteAllFonts();
		void deleteAllTexts();
		void deleteAllImages();

		void deleteAll();

		Map<String, AudioStream> getAudioStreams();
		Map<String, Font> getFonts();
		Map<String, Text> getTexts();
		Map<String, Image> getImages();
	private:
		Map<String, AudioStream> audioStreams;
		Map<String, Font> fonts;
		Map<String, Text> texts;
		Map<String, Image> images;
	};
}