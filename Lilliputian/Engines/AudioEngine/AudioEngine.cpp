#include "AudioEngine.hpp"
#include "AudioSDL/AudioSDL.hpp"

Lilliputian::AudioEngine::AudioEngine()
{

}

void Lilliputian::AudioEngine::play()
{
	Audio::SDL::playSounds(&this->immediateSounds, &this->scheduledSounds);
}

void Lilliputian::AudioEngine::process(Scene* scene)
{
	//todo: build sounds lists from scene
}