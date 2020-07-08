#include "Audio.hpp"

Lilliputian::Audio::Audio(const char* filepath)
{
	this->sound = Mix_LoadWAV(filepath);
}

void Lilliputian::Audio::unload()
{
	Mix_FreeChunk(this->sound);
}