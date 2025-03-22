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
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include "scene/components/audio_listener.hpp"
#include <scene/components/audio_source.hpp>
#include <foundations/singletons/event_bus.hpp>

Omnific::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnific::AudioSystem::initialize()
{
	this->is_initialized = true;
	this->alc_device = alcOpenDevice(NULL);
	if (this->alc_device)
	{
		this->alc_context = alcCreateContext(this->alc_device, NULL);
		alcMakeContextCurrent(this->alc_context);
	}
	Platform::get_logger().write("Initialized Audio System.");
}

void Omnific::AudioSystem::on_update(std::shared_ptr<Scene> scene)
{
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::vector<std::shared_ptr<AudioSource>> audio_sources = scene_layer_it.second->get_components_by_type<AudioSource>();

		/* Update the playback times on AudioSources*/
		for (int i = 0; i < audio_sources.size(); i++)
		{
			std::shared_ptr<AudioSource> audio_source = audio_sources.at(i);
			std::shared_ptr<Audio> active_audio = audio_source->get_active_audio();
			//audio_source->playback_time = Mix_GetMusicPosition(active_audio_stream->get_sdlmix_music().get());
		}
	}
}

void Omnific::AudioSystem::on_output(std::shared_ptr<Scene> scene)
{
	std::map<SceneLayerID, std::shared_ptr<SceneLayer>>& scene_layers = scene->get_scene_layers();

	for (auto it = scene_layers.begin(); it != scene_layers.end(); it++)
	{
		std::vector<std::shared_ptr<AudioListener>> audio_listeners = it->second->get_components_by_type<AudioListener>();

		if (audio_listeners.size() > 0)
		{
			std::shared_ptr<AudioListener> audio_listener = audio_listeners.at(audio_listeners.size() - 1); //Get the last AudioListener.
			std::vector<std::shared_ptr<AudioSource>> audio_sources = it->second->get_components_by_type<AudioSource>();

			//Map Audio Sources to ComponentIDs for faster search by EventBus
			std::unordered_map<UID, std::shared_ptr<AudioSource>> mapped_audio_sources;
			for (int i = 0; i < audio_sources.size(); i++)
			{
				std::shared_ptr<AudioSource> audio_source = audio_sources.at(i);
				mapped_audio_sources.emplace(audio_source->get_id(), audio_source);
			}

			//Event Responses

			for (Event playing_event : EventBus::query_events("playing_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, playing_event);

			}

			for (Event pausing_event : EventBus::query_events("pausing_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, pausing_event);

			}

			for (Event resuming_event : EventBus::query_events("resuming_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, resuming_event);

			}

			for (Event stopping_event : EventBus::query_events("stopping_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, stopping_event);

			}

			for (Event reseting_event : EventBus::query_events("reseting_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, reseting_event);

			}

			for (Event jumping_event : EventBus::query_events("jumping_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, jumping_event);
				double time_point = jumping_event.get_parameters().numbers["time_point"];

			}

			for (Event panned_event : EventBus::query_events("panned_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, panned_event);

				std::shared_ptr<AudioSource> audio_source = mapped_audio_sources.at(panned_event.get_parameters().numbers["id"]);
				Uint8 left = 0;
				Uint8 right = 0;

				if (audio_source->panning > 0.0)
					left = (Uint8)(audio_source->panning * 255.0);
				else
					right = (Uint8)(-audio_source->panning * 255.0);

			}
		}
	}
}

void Omnific::AudioSystem::finalize()
{
	if (this->is_initialized)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(this->alc_context);
		alcCloseDevice(this->alc_device);
	}

	this->is_initialized = false;
}

std::shared_ptr<Omnific::Audio> Omnific::AudioSystem::query_active_audio_by_event(
	std::unordered_map<UID, std::shared_ptr<AudioSource>> mapped_audio_sources,
	Event audio_event)
{
	std::shared_ptr<Audio> active_audio;

	ComponentID audio_source_id = (ComponentID)audio_event.get_parameters().numbers["id"];
	if (mapped_audio_sources.count(audio_source_id) > 0)
	{
		std::shared_ptr<AudioSource> audio_source = mapped_audio_sources[audio_source_id];
		active_audio = audio_source->get_active_audio();
	}

	return active_audio;
}