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
#include <SDL_mixer.h>

void Omnia::AudioSource::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		std::string audio_type = it3->first.as<std::string>();

		if (audio_type == "AudioStreams")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioStream> audio_stream(OS::get_file_access().load_asset_by_type<AudioStream>(it3->second[i].as<std::string>(), false));
				this->add_audio(audio_stream);
			}
		}
		else if (audio_type == "AudioSyntheses")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioSynthesis> audio_synthesis(OS::get_file_access().load_asset_by_type<AudioSynthesis>(it3->second[i].as<std::string>(), false));
				this->add_audio(audio_synthesis);
			}
		}
	}
}

void Omnia::AudioSource::add_audio(std::shared_ptr<Audio> audio)
{
	this->audio_collection.emplace(audio->get_name(), audio);
}

void Omnia::AudioSource::set_volume(float value)
{
	if (this->active_audio_name != "")
	{
		value = this->normalize(value);

		if (this->is_playing_synthesized)
		{
			libretti->playback->outputVolume = value;
		}
		else
		{
			Mix_MasterVolume(value * 128);
		}
	}
}

float Omnia::AudioSource::get_volume()
{
	float volume = 0.0;

	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			volume = libretti->playback->outputVolume;
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream = std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));
			Mix_Music* music = audio_stream->get_sdlmix_music().get();
			volume = Mix_GetMusicVolume(music);
		}
	}

	return volume;
}

void Omnia::AudioSource::clear_audio()
{
	this->audio_collection.clear();
}

void Omnia::AudioSource::remove_audio(std::string audio_name)
{
	this->audio_collection.erase(audio_name);
}

void Omnia::AudioSource::play_audio(std::string audio_name)
{
	this->stop();

	if (this->audio_collection.count(audio_name))
		this->active_audio_name = audio_name;
	else
		this->active_audio_name = "";

	if (this->active_audio_name != "")
	{
		std::shared_ptr<Audio> audio = this->audio_collection.at(this->active_audio_name);

		if (audio->is_type(AudioSynthesis::TYPE_STRING))
		{
			std::shared_ptr<AudioSynthesis> audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(audio);
			this->libretti = lb_createEmptyLibretti();
			this->libretti->composition = audio_synthesis->get_composition();
			lb_addLibrettiToCallback(libretti);
			this->is_playing_synthesized = true;
		}
		else if (audio->is_type(AudioStream::TYPE_STRING))
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
			{
				Mix_PlayMusic(audio_stream->get_sdlmix_music().get(), 1);
			}
			else
			{
				Mix_PlayChannel(-1, audio_stream->get_sdlmix_chunk().get(), 1);
			}
		}
	}
}

void Omnia::AudioSource::play()
{
	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			lb_play(this->libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream = 
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
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
	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			lb_pause(this->libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
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
	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			lb_stop(libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
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
	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			lb_reset(libretti);
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
			{
				Mix_RewindMusic();
			}
		}
	}
}

uint16_t Omnia::AudioSource::get_repeat_count()
{
	uint16_t repeat_count = 0;
	return repeat_count;
}

void Omnia::AudioSource::jump(float time_point)
{
	if (this->active_audio_name != "")
	{
		if (time_point >= 0.0 && time_point <= this->get_active_audio()->get_playback_length())
		{
			if (this->is_playing_synthesized)
			{
				libretti->playback->currentPlayTime = time_point;
			}
			else
			{
				std::shared_ptr<AudioStream> audio_stream =
					std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

				if (audio_stream->get_is_music())
				{
					Mix_SetMusicPosition(time_point);
				}
			}
		}
	}
}

void Omnia::AudioSource::set_panning(float value)
{
	if (value < -1.0)
		value = -1.0;
	else if (value > 1.0)
		value = 1.0;

	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			;
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
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

float Omnia::AudioSource::get_current_playback_time()
{
	float current_playback_time = 0.0;

	if (this->active_audio_name != "")
	{
		if (this->is_playing_synthesized)
		{
			current_playback_time = this->libretti->playback->currentPlayTime;
		}
		else
		{
			std::shared_ptr<AudioStream> audio_stream =
				std::dynamic_pointer_cast<AudioStream>(this->audio_collection.at(this->active_audio_name));

			if (audio_stream->get_is_music())
			{
				current_playback_time = Mix_GetMusicPosition(audio_stream->get_sdlmix_music().get());
			}
		}
	}

	return current_playback_time;
}

std::vector<std::string> Omnia::AudioSource::get_audio_names()
{
	std::vector<std::string> audio_stream_names;

	for (std::unordered_map<std::string, std::shared_ptr<Audio>>::iterator it = this->audio_collection.begin();
		it != this->audio_collection.end();
		++it)
	{
		audio_stream_names.push_back(it->first);
	}

	return audio_stream_names;
}

std::shared_ptr<Omnia::Audio> Omnia::AudioSource::get_active_audio()
{
	std::shared_ptr<Audio> active_audio;

	if (this->active_audio_name != "")
	{
		active_audio = this->get_audio_by_name(this->active_audio_name);
	}

	return active_audio;
}

std::shared_ptr<Omnia::Audio> Omnia::AudioSource::get_audio_by_name(std::string audio_name)
{
	std::shared_ptr<Audio> audio_stream;

	if (this->audio_collection.count(audio_name))
		audio_stream = this->audio_collection.at(audio_name);

	return audio_stream;
}

float Omnia::AudioSource::normalize(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	return value;
}