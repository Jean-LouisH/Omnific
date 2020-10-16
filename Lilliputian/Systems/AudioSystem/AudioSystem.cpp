#include "AudioSystem.hpp"
#include "AudioSDL/AudioSDL.hpp"

Lilliputian::AudioSystem::AudioSystem()
{
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, pow(2, 11));
}

Lilliputian::AudioSystem::~AudioSystem()
{
	Mix_CloseAudio();
	Mix_Quit();
}

void Lilliputian::AudioSystem::play()
{
	SDL::Audio::playSounds(&this->immediateSounds, &this->scheduledSounds);
}

void Lilliputian::AudioSystem::process(Scene& scene)
{
	//todo: build sounds lists from scene
}