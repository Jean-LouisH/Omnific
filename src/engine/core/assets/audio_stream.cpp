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
#include "core/singletons/os/os.hpp"

Omnia::AudioStream::~AudioStream()
{

}

Omnia::AudioStream::AudioStream(std::string filepath)
{
	this->setName(filepath);
	this->type = TYPE_STRING;

	std::string fileExtension = OS::getFileAccess().getFileExtension(filepath);

	if (fileExtension == "mp3" || 
		fileExtension == "ogg")
	{
		this->isMusic = true;
		this->music = std::shared_ptr<Mix_Music>(Mix_LoadMUS(filepath.c_str()), Mix_FreeMusic);
	}
	else if (fileExtension == "wav")
	{
		this->isMusic = false;
		this->soundFX = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(filepath.c_str()), Mix_FreeChunk);
	}
}

float Omnia::AudioStream::getPlaybackLength()
{
	float playbackLength = 0.0;

	if (isMusic)
	{
		playbackLength = Mix_MusicDuration(this->music.get());
	}

	return playbackLength;
	
}

std::vector<uint16_t> Omnia::AudioStream::getSpectrumData()
{
	std::vector<uint16_t> spectrum;
	return spectrum;
}

std::shared_ptr<Mix_Chunk> Omnia::AudioStream::getSDLMixChunk()
{
	return this->soundFX;
}

std::shared_ptr<Mix_Music> Omnia::AudioStream::getSDLMixMusic()
{
	return this->music;
}

bool Omnia::AudioStream::getIsMusic()
{
	return this->isMusic;
}