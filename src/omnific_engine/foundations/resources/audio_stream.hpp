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

#include <SDL_mixer.h>
#include "audio.hpp"
#include <string>
#include <memory>

namespace Omnific
{
	class OMNIFIC_ENGINE_API AudioStream : public Audio
	{
	public:
		static constexpr const char* TYPE_STRING = "AudioStream";
		AudioStream() 
		{ 
			this->type = TYPE_STRING; 
		};
		~AudioStream();
		AudioStream(std::string filepath);

		virtual Registerable* instance() override
		{
			AudioStream* clone = new AudioStream(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		std::shared_ptr<Mix_Chunk> get_sdlmix_chunk();
		std::shared_ptr<Mix_Music> get_sdlmix_music();
		bool get_is_music();

		float get_playback_length() override;
		std::vector<uint16_t> get_spectrum_data() override;
	private:
		bool is_music = false;
		std::shared_ptr<Mix_Music> music = {nullptr, Mix_FreeMusic};
		std::shared_ptr<Mix_Chunk> sound_fx = {nullptr, Mix_FreeChunk};
	};
}