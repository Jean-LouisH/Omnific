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

#include "audio_system.hpp"
#include "audio_sdl/audio_sdl.hpp"

#include <scene/components/audio_source.hpp>

Omnia::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnia::AudioSystem::play()
{
	SDL::Audio::playSoundFXs(&this->soundFXQueue);
	SDL::Audio::playMusic(&this->musicQueue);
}

void Omnia::AudioSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, pow(2, 11));
	this->isInitialized = true;
}

void Omnia::AudioSystem::onLate(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		/* Basic functionality for now, without 3D audio listener calculations. */
		std::vector<std::shared_ptr<AudioSource>> audioSources = it->second->getComponentsByType<AudioSource>();

		for (size_t i = 0; i < audioSources.size(); i++)
		{
			std::queue<std::shared_ptr<AudioStream>> audioPlayQueue = audioSources.at(i)->popEntireAudioPlayQueue();

			while (!audioPlayQueue.empty())
			{
				std::shared_ptr<AudioStream> audioStream = audioPlayQueue.front();

				if (audioStream->getIsMusic())
					this->musicQueue.emplace(audioStream->getSDLMixMusic());
				else
					this->soundFXQueue.emplace(audioStream->getSDLMixChunk());

				audioPlayQueue.pop();
			}
		}
	}

	this->play();
}

void Omnia::AudioSystem::finalize()
{
	if (this->isInitialized)
	{
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	this->isInitialized = false;
}