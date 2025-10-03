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

#include "foundations/resources/resource.hpp"
#include <string>
#include <memory>
#include <stdint.h>
#include <vector>

namespace Omnific
{
	class OMNIFIC_ENGINE_API AudioClip : public Resource
	{
		friend class AudioSystem;
	public:
		static constexpr const char* TYPE_STRING = "AudioClip";
		AudioClip()
		{
			this->type = TYPE_STRING;
		};

		AudioClip(std::string filepath);
		AudioClip(std::vector<int16_t> data, int channel_count, int sample_rate, int samples_per_channel);

		virtual Registerable* instance() override
		{
			AudioClip* clone = new AudioClip(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		int get_sample_rate();
		int get_channel_count();
		float get_playback_length();
		std::vector<int16_t> get_spectrum();

		std::vector<int16_t> data;
	private:
		int sample_rate = 0;
		int channel_count = 0;
		int samples_per_channel = 0;
		float playback_length = 0.0;
		int bytes_per_sample = sizeof(int16_t);
	};
}