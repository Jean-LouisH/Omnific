#include "streamed_audio.hpp"


Lilliputian::StreamedAudio::StreamedAudio()
{

}

Lilliputian::StreamedAudio::StreamedAudio(const char* filepath)
{
	this->sound = Mix_LoadWAV(filepath);
}

Mix_Chunk* Lilliputian::StreamedAudio::getSDLMixChunk()
{
	return this->sound;
}

void Lilliputian::StreamedAudio::unload()
{
	Mix_FreeChunk(this->sound);
}