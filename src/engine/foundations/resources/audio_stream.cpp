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

#include "audio_stream.hpp"
#include "foundations/singletons/platform/platform.hpp"

Omnia::AudioStream::~AudioStream()
{

}

Omnia::AudioStream::AudioStream(std::string filepath)
{
	this->set_name(filepath);
	this->type = TYPE_STRING;

	std::string file_extension = Platform::get_file_access().get_file_extension(filepath);

	if (file_extension == "mp3" || 
		file_extension == "ogg")
	{
		this->is_music = true;
		this->music = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filepath.c_str()), Mix_FreeMusic);
	}
	else if (file_extension == "wav")
	{
		this->is_music = false;
		this->sound_fx = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(filepath.c_str()), Mix_FreeChunk);
	}
}

float Omnia::AudioStream::get_playback_length()
{
	float playback_length = 0.0;

	if (is_music)
	{
		playback_length = Mix_MusicDuration(this->music.get());
	}

	return playback_length;
	
}

std::vector<uint16_t> Omnia::AudioStream::get_spectrum_data()
{
	std::vector<uint16_t> spectrum;
	return spectrum;
}

std::shared_ptr<Mix_Chunk> Omnia::AudioStream::get_sdlmix_chunk()
{
	return this->sound_fx;
}

std::shared_ptr<Mix_Music> Omnia::AudioStream::get_sdlmix_music()
{
	return this->music;
}

bool Omnia::AudioStream::get_is_music()
{
	return this->is_music;
}