#include "Audio.hpp"

Lilliputian::Audio::Audio(const char* filepath)
{
	this->sound = Mix_LoadWAV(filepath);
}

Lilliputian::Audio::~Audio()
{
	Mix_FreeChunk(this->sound);
}