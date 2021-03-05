#include "audio_stream.hpp"


Lilliputian::AudioStream::AudioStream()
{

}

Lilliputian::AudioStream::AudioStream(const char* filepath)
{
	this->sound = Mix_LoadWAV(filepath);
}

Mix_Chunk* Lilliputian::AudioStream::getSDLMixChunk()
{
	return this->sound;
}

void Lilliputian::AudioStream::unload()
{
	Mix_FreeChunk(this->sound);
}