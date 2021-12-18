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

#include <unordered_map>
#include <string>
#include "assets/audio_stream.hpp"
#include "assets/font.hpp"
#include "assets/text.hpp"
#include "assets/image.hpp"

namespace Esi
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

		std::unordered_map<std::string, AudioStream> getAudioStreams();
		std::unordered_map<std::string, Font> getFonts();
		std::unordered_map<std::string, Text> getTexts();
		std::unordered_map<std::string, Image> getImages();
	private:
		std::unordered_map<std::string, AudioStream> audioStreams;
		std::unordered_map<std::string, Font> fonts;
		std::unordered_map<std::string, Text> texts;
		std::unordered_map<std::string, Image> images;
	};
}