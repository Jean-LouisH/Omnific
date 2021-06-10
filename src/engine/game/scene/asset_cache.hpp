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

#include <map>
#include "utilities/string.hpp"
#include "assets/audio_stream.hpp"
#include "assets/font.hpp"
#include "assets/text.hpp"
#include "assets/image.hpp"

namespace Lilliputian
{
	class AssetCache
	{
	public:
		AudioStream loadAudioStream(const char* filepath);
		Font loadFont(const char* filepath, uint16_t size_px);
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

		std::map<String, AudioStream> getAudioStreams();
		std::map<String, Font> getFonts();
		std::map<String, Text> getTexts();
		std::map<String, Image> getImages();
	private:
		std::map<String, AudioStream> audioStreams;
		std::map<String, Font> fonts;
		std::map<String, Text> texts;
		std::map<String, Image> images;
	};
}