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
#include "foundations/resources/audio_stream.hpp"
#include "foundations/resources/audio_synthesis.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include "foundations/singletons/event_bus.hpp"

void Omnific::AudioSource::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		std::string audio_type = it3->first.as<std::string>();

		if (audio_type == "audio_streams")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioStream> audio_stream(Platform::get_file_access().load_resource_by_type<AudioStream>(it3->second[i].as<std::string>(), false));
				this->add_audio(audio_stream);
			}
		}
		else if (audio_type == "audio_syntheses")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<AudioSynthesis> audio_synthesis(Platform::get_file_access().load_resource_by_type<AudioSynthesis>(it3->second[i].as<std::string>(), false));
				this->add_audio(audio_synthesis);
			}
		}
	}
}

void Omnific::AudioSource::add_audio(std::shared_ptr<Audio> audio)
{
	this->audio_collection.emplace(audio->get_name(), audio);
}

void Omnific::AudioSource::set_volume(float value)
{
	this->volume = this->clamp(value);
}

float Omnific::AudioSource::get_volume()
{
	return this->volume;
}

void Omnific::AudioSource::clear_audio()
{
	this->audio_collection.clear();
}

void Omnific::AudioSource::remove_audio(std::string audio_name)
{
	this->audio_collection.erase(audio_name);
}

void Omnific::AudioSource::play_audio(std::string audio_name)
{
	this->stop();

	if (this->audio_collection.count(audio_name))
		this->active_audio_name = audio_name;
	else
		this->active_audio_name = "";

	if (this->active_audio_name != "")
	{
		this->playback_state = PlaybackState::PLAYING;
		EventBus::publish("playing_audio_source", {}, {{"id", this->get_id()}});
	}
}

void Omnific::AudioSource::play()
{
	this->play_audio(this->active_audio_name);
}

void Omnific::AudioSource::pause()
{
	if (this->active_audio_name != "")
	{
		this->playback_state = PlaybackState::PAUSED;
		EventBus::publish("pausing_audio_source", {}, {{"id", this->get_id()}});
	}
}

void Omnific::AudioSource::resume()
{
	if (this->active_audio_name != "")
	{
		this->playback_state = PlaybackState::PLAYING;
		EventBus::publish("resuming_audio_source", {}, {{"id", this->get_id()}});
	}
}

void Omnific::AudioSource::stop()
{
	this->playback_state = PlaybackState::STOPPED;
	EventBus::publish("stopping_audio_source", {}, {{"id", this->get_id()}});
}

void Omnific::AudioSource::reset()
{
	if (this->active_audio_name != "")
	{
		EventBus::publish("reseting_audio_source", {}, {{"id", this->get_id()}});
	}
}

void Omnific::AudioSource::jump(float time_point)
{
	if (this->active_audio_name != "")
	{
		if (time_point >= 0.0 && time_point <= this->get_active_audio()->get_playback_length())
		{
			EventBus::publish("jumping_audio_source", {}, {
				{"id", this->get_id()},
				{"time_point", time_point} });
		}
	}
}

void Omnific::AudioSource::set_panning(float value)
{
	if (value < -1.0)
		value = -1.0;
	else if (value > 1.0)
		value = 1.0;

	this->panning = value;
	EventBus::publish("panned_audio_source", {}, {{"id", this->get_id()}});
}

float Omnific::AudioSource::get_current_playback_time()
{
	return this->playback_time;
}

std::vector<std::string> Omnific::AudioSource::get_audio_names()
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

std::shared_ptr<Omnific::Audio> Omnific::AudioSource::get_active_audio()
{
	std::shared_ptr<Audio> active_audio;

	if (this->active_audio_name != "")
		active_audio = this->get_audio_by_name(this->active_audio_name);

	return active_audio;
}

std::shared_ptr<Omnific::Audio> Omnific::AudioSource::get_audio_by_name(std::string audio_name)
{
	std::shared_ptr<Audio> audio_stream;

	if (this->audio_collection.count(audio_name))
		audio_stream = this->audio_collection.at(audio_name);

	return audio_stream;
}

float Omnific::AudioSource::clamp(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	return value;
}