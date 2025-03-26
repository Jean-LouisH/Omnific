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

#include "audio.hpp"
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
#include <foundations/singletons/platform/platform.hpp>
#include <stb_vorbis.c>

Omnific::Audio::Audio(std::string filepath)
{
	this->type = TYPE_STRING;
	std::string file_extension = Platform::get_file_access().get_file_extension(filepath);
	std::string full_filepath = Platform::get_file_access().find_path(filepath);
	short* data_output;

	if (file_extension == "ogg")
	{
		this->samples_per_channel = stb_vorbis_decode_filename(full_filepath.c_str(), &this->channel_count, &this->sample_rate, &data_output);
		this->data = std::vector<int16_t>(data_output, data_output + this->samples_per_channel * this->channel_count);
		free(data_output);
	}
	else if (file_extension == "wav")
	{
		unsigned int* channels;
		unsigned int* sample_rate;
		drwav_uint64* total_frame_count;
		data_output = (short*)drwav_open_file_and_read_pcm_frames_s16(filepath.c_str(), channels, sample_rate, total_frame_count, NULL);
		this->channel_count = (int)*channels;
		this->sample_rate = (int)*sample_rate;
		this->samples_per_channel = (int)*total_frame_count;
		this->data = std::vector<int16_t>(data_output, data_output + this->samples_per_channel * this->channel_count);
		drwav_free(data_output, NULL);
	}

	this->playback_length = this->samples_per_channel / this->sample_rate;
	this->size = this->samples_per_channel * this->channel_count * sizeof(int16_t);
}

int Omnific::Audio::get_sample_rate()
{
	return this->sample_rate;
}

int Omnific::Audio::get_channel_count()
{
	return this->channel_count;
}

float Omnific::Audio::get_playback_length()
{
	return this->playback_length;
}

std::vector<int16_t> Omnific::Audio::get_spectrum()
{
	return this->data;
}