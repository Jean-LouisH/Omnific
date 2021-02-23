#include "streamed_audio.hpp"

Lilliputian::StreamedAudio::StreamedAudio(const char* filepath)
{
	this->sound = Mix_LoadWAV(filepath);
}

void Lilliputian::StreamedAudio::unload()
{
	Mix_FreeChunk(this->sound);
}