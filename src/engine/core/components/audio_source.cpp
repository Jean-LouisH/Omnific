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

#include "audio_source.hpp"
#include "core/assets/audio_stream.hpp"
#include "core/assets/audio_synthesis.hpp"
#include "core/singletons/os/os.hpp"

void Omnia::AudioSource::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		std::string audioType = it3->first.as<std::string>();

		if (audioType == "AudioStreams")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioStream> audioStream(OS::getFileAccess().loadAssetByType<AudioStream>(it3->second[i].as<std::string>(), false));
				this->addAudio(audioStream);
			}
		}
		else if (audioType == "AudioSyntheses")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioSynthesis> audioSynthesis(OS::getFileAccess().loadAssetByType<AudioSynthesis>(it3->second[i].as<std::string>(), false));
				this->addAudio(audioSynthesis);
			}
		}
	}
}

void Omnia::AudioSource::addAudio(std::shared_ptr<Audio> audio)
{
	this->audioCollection.emplace(audio->getName(), audio);
}

void Omnia::AudioSource::setVolume(float value)
{
	if (this->activeAudioName != "")
	{
		value = this->normalize(value);

		if (this->isPlayingSynthesized)
		{
			libretti->playback->userEffectsOverride.outputVolume = value;
		}

		Mix_MasterVolume(value * 128);
	}
}

float Omnia::AudioSource::getVolume()
{
	float volume = 0.0;

	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			volume = libretti->playback->userEffectsOverride.outputVolume;
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream = std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));
			Mix_Music* music = audioStream->getSDLMixMusic().get();
			volume = Mix_GetMusicVolume(music);
		}
	}

	return volume;
}

void Omnia::AudioSource::clearAudio()
{
	this->audioCollection.clear();
}

void Omnia::AudioSource::removeAudio(std::string audioName)
{
	this->audioCollection.erase(audioName);
}

void Omnia::AudioSource::playAudio(std::string audioName)
{
	this->stop();

	if (this->audioCollection.count(audioName))
		this->activeAudioName = audioName;
	else
		this->activeAudioName = "";

	if (this->activeAudioName != "")
	{
		std::shared_ptr<Audio> audio = this->audioCollection.at(this->activeAudioName);

		if (audio->isType(AudioSynthesis::TYPE_STRING))
		{
			std::shared_ptr<AudioSynthesis> audioSynthesis = std::dynamic_pointer_cast<AudioSynthesis>(audio);
			this->libretti = lb_createEmptyLibretti();
			this->libretti->composition = audioSynthesis->getComposition();
			lb_addLibrettiToCallback(libretti);
			this->isPlayingSynthesized = true;
		}
		else if (audio->isType(AudioStream::TYPE_STRING))
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Mix_PlayMusic(audioStream->getSDLMixMusic().get(), 1);
			}
			else
			{
				Mix_PlayChannel(-1, audioStream->getSDLMixChunk().get(), 1);
			}
		}
	}
}

void Omnia::AudioSource::play()
{
	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			lb_play(this->libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream = 
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_Resume(-1);
			}
		}
	}
}

void Omnia::AudioSource::pause()
{
	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			lb_pause(this->libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Mix_PauseMusic();
			}
			else
			{
				Mix_Pause(-1);
			}
		}
	}
}

void Omnia::AudioSource::stop()
{
	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			lb_stop(libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Mix_HaltMusic();
			}
			else
			{
				Mix_HaltChannel(-1);
			}
		}
	}
}

void Omnia::AudioSource::reset()
{
	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			lb_reset(libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Mix_RewindMusic();
			}
		}
	}
}

uint16_t Omnia::AudioSource::getRepeatCount()
{
	uint16_t repeatCount = 0;
	return repeatCount;
}

void Omnia::AudioSource::jump(float timePoint)
{
	if (this->activeAudioName != "")
	{
		if (timePoint >= 0.0 && timePoint <= this->getActiveAudio()->getPlaybackLength())
		{
			if (this->isPlayingSynthesized)
			{
				libretti->playback->currentPlayTime = timePoint;
			}
			else
			{
				std::shared_ptr<AudioStream> audioStream =
					std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

				if (audioStream->getIsMusic())
				{
					Mix_SetMusicPosition(timePoint);
				}
			}
		}
	}
}

void Omnia::AudioSource::setPanning(float value)
{
	if (value < -1.0)
		value = -1.0;
	else if (value > 1.0)
		value = 1.0;

	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			libretti->playback->userEffectsOverride.outputPanning = value;
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				Uint8 left = 0;
				Uint8 right = 0;

				if (value > 0.0)
				{
					left = (Uint8)(value * 255.0);
				}
				else
				{
					right = (Uint8)(-value * 255.0);
				}

				Mix_SetPanning(MIX_CHANNEL_POST, left, right);
			}
		}
	}
}

float Omnia::AudioSource::getCurrentPlaybackTime()
{
	float currentPlaybackTime = 0.0;

	if (this->activeAudioName != "")
	{
		if (this->isPlayingSynthesized)
		{
			currentPlaybackTime = this->libretti->playback->currentPlayTime;
		}
		else
		{
			std::shared_ptr<AudioStream> audioStream =
				std::dynamic_pointer_cast<AudioStream>(this->audioCollection.at(this->activeAudioName));

			if (audioStream->getIsMusic())
			{
				currentPlaybackTime = Mix_GetMusicPosition(audioStream->getSDLMixMusic().get());
			}
		}
	}

	return currentPlaybackTime;
}

std::vector<std::string> Omnia::AudioSource::getAudioNames()
{
	std::vector<std::string> audioStreamNames;

	for (std::unordered_map<std::string, std::shared_ptr<Audio>>::iterator it = this->audioCollection.begin();
		it != this->audioCollection.end();
		++it)
	{
		audioStreamNames.push_back(it->first);
	}

	return audioStreamNames;
}

std::shared_ptr<Omnia::Audio> Omnia::AudioSource::getActiveAudio()
{
	std::shared_ptr<Audio> activeAudio;

	if (this->activeAudioName != "")
	{
		activeAudio = this->getAudioByName(this->activeAudioName);
	}

	return activeAudio;
}

std::shared_ptr<Omnia::Audio> Omnia::AudioSource::getAudioByName(std::string audioName)
{
	std::shared_ptr<Audio> audioStream;

	if (this->audioCollection.count(audioName))
		audioStream = this->audioCollection.at(audioName);

	return audioStream;
}

float Omnia::AudioSource::normalize(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	return value;
}