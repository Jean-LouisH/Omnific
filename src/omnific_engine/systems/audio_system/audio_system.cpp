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
#include <foundations/resources/audio_stream.hpp>
#include <Libretti.h>
#include "scene/components/audio_listener.hpp"
#include <scene/components/audio_source.hpp>
#include <foundations/singletons/event_bus.hpp>

Omnific::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnific::AudioSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, pow(2, 11));
	lb_initialize();
	this->libretti = lb_createEmptyLibretti();
	this->is_initialized = true;
	Platform::get_logger().write("Initialized Audio System.");
}

void Omnific::AudioSystem::on_logic(std::shared_ptr<Scene> scene)
{
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::vector<std::shared_ptr<AudioSource>> audio_sources = scene_layer_it.second->get_components_by_type<AudioSource>();

		for (int i = 0; i < audio_sources.size(); i++)
		{
			std::shared_ptr<AudioSource> audio_source = audio_sources.at(i);
			std::shared_ptr<Audio> active_audio = audio_source->get_active_audio();

			if (active_audio != nullptr)
			{
				if (active_audio->is_type(AudioStream::TYPE_STRING))
				{
					std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
					audio_source->playback_time = Mix_GetMusicPosition(active_audio_stream->get_sdlmix_music().get());
				}
				else if (active_audio->is_type(AudioSynthesis::TYPE_STRING))
				{
					audio_source->playback_time = this->libretti->playback->currentPlayTime;
				}
			}
		}
	}

	lb_incrementAllPlayTimes(Profiler::get_timer(LOOP_THREAD_TIMER_NAME)->get_delta_in_seconds());
}

void Omnific::AudioSystem::on_late(std::shared_ptr<Scene> scene)
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

			//Event Resposes

			for (Event playing_event : EventBus::query("playing_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, playing_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
						Mix_PlayMusic(active_audio_stream->get_sdlmix_music().get(), 1);
					else
						Mix_PlayChannel(-1, active_audio_stream->get_sdlmix_chunk().get(), 1);
				}
				else if (active_audio_synthesis != nullptr)
				{
					this->libretti = lb_createEmptyLibretti();
					this->libretti->composition = active_audio_synthesis->get_composition();
					lb_addLibrettiToCallback(libretti);
				}
			}

			for (Event pausing_event : EventBus::query("pausing_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, pausing_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
						Mix_PauseMusic();
					else
						Mix_Pause(-1);
				}
				else if (active_audio_synthesis != nullptr)
				{
					lb_pause(this->libretti);
				}
			}

			for (Event resuming_event : EventBus::query("resuming_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, resuming_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
						Mix_ResumeMusic();
					else
						Mix_Resume(-1);
				}
				else if (active_audio_synthesis != nullptr)
				{
					lb_play(this->libretti);
				}
			}

			for (Event stopping_event : EventBus::query("stopping_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, stopping_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
						Mix_HaltMusic();
					else
						Mix_HaltChannel(-1);
				}
				else if (active_audio_synthesis != nullptr)
				{
					lb_stop(libretti);
				}
			}

			for (Event reseting_event : EventBus::query("reseting_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, reseting_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
						Mix_RewindMusic();
				}
				else if (active_audio_synthesis != nullptr)
				{
					lb_reset(libretti);
				}
			}

			for (Event jumping_event : EventBus::query("jumping_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, jumping_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);
				double time_point = jumping_event.get_parameters().numbers["time_point"];

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
					{
						Mix_SetMusicPosition(time_point);
					}
				}
				else if (active_audio_synthesis != nullptr)
				{
					libretti->playback->currentPlayTime = time_point;
				}
			}

			for (Event panned_event : EventBus::query("panned_audio_source"))
			{
				std::shared_ptr<Audio> active_audio = this->query_active_audio_by_event(mapped_audio_sources, panned_event);
				std::shared_ptr<AudioStream> active_audio_stream = std::dynamic_pointer_cast<AudioStream>(active_audio);
				std::shared_ptr<AudioSynthesis> active_audio_synthesis = std::dynamic_pointer_cast<AudioSynthesis>(active_audio);

				if (active_audio_stream != nullptr)
				{
					if (active_audio_stream->get_is_music())
					{
						std::shared_ptr<AudioSource> audio_source = mapped_audio_sources.at(panned_event.get_parameters().numbers["id"]);
						Uint8 left = 0;
						Uint8 right = 0;

						if (audio_source->panning > 0.0)
							left = (Uint8)(audio_source->panning * 255.0);
						else
							right = (Uint8)(-audio_source->panning * 255.0);

						Mix_SetPanning(MIX_CHANNEL_POST, left, right);
					}
				}
				else if (active_audio_synthesis != nullptr)
				{

				}
			}

			Mix_MasterVolume(audio_listener->get_volume() * 128);	
		}
	}
}

void Omnific::AudioSystem::finalize()
{
	if (this->is_initialized)
	{
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
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