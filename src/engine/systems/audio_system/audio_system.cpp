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
#include <foundations/singletons/configuration.hpp>
#include "scene/components/audio_listener.hpp"
#include <scene/components/audio_source.hpp>
#include <foundations/singletons/event_bus.hpp>
#include <foundations/resources/audio.hpp>
#include <scene/components/physics_body.hpp>
#include <foundations/singletons/scene_manager.hpp>
#include <SDL3/SDL.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#define AUDIO_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "audio_system_on_output_frame_time"

Omnific::AudioSystem::AudioSystem()
{
	this->type = TYPE_STRING;
	this->mix_buffer.resize(this->mix_samples_per_frame);
}

Omnific::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnific::AudioSystem::initialize()
{
	Profiler::add_clock(AUDIO_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"audio_system", "on_output_frame_time"});

	if (!SDL_Init(SDL_INIT_AUDIO)) 
	{
        Platform::get_logger().write_error("SDL_Init failed: " + std::string(SDL_GetError()), Logger::Category::AUDIO);
        return;
    }

	this->is_initialized = true;
	this->device_id = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);

	if (this->device_id == 0) 
	{
		Platform::get_logger().write_error("Failed to open audio: " + std::string(SDL_GetError()), Logger::Category::AUDIO);
		return;
	}

	SDL_AudioSpec spec;
	SDL_zero(spec);
	spec.freq = this->mix_sample_frequency;
	spec.format = SDL_AUDIO_S16;
	spec.channels = this->mix_channel_count;

	this->audio_stream = SDL_CreateAudioStream(&spec, &spec);
	SDL_BindAudioStream(this->device_id, this->audio_stream);
	SDL_ResumeAudioDevice(this->device_id);

	Platform::get_logger().write("Initialized Audio System.");
}

void Omnific::AudioSystem::on_output()
{
	std::shared_ptr<Scene> scene = SceneManager::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(AUDIO_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (EventBus::has_event(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED))
	{
		SDL_ClearAudioStream(this->audio_stream);
	}

	const int queue_refill_threshold = this->mix_samples_per_frame * this->bytes_per_sample * 8;
	int queued_audio_size = SDL_GetAudioStreamQueued(this->audio_stream);

	if (queued_audio_size < queue_refill_threshold)
	{
		std::vector<float> temp_cumulative_buffer(this->mix_samples_per_frame, 0.0f);

		std::vector<std::shared_ptr<AudioListener>> audio_listeners = scene->get_components_by_type<AudioListener>();

		if (audio_listeners.size() == 0)
		{
			SDL_PauseAudioDevice(this->device_id);
		}
		else
		{
			SDL_ResumeAudioDevice(this->device_id);

			for (std::shared_ptr<AudioListener>& audio_listener : scene->get_components_by_type<AudioListener>())
			{
				std::shared_ptr<Entity> listener_entity = scene->get_entity(audio_listener->get_entity_id());
				std::shared_ptr<Transform> listener_global_transform = scene->calculate_global_transform(listener_entity->get_id());
				std::shared_ptr<PhysicsBody> listener_physics_body = scene->get_component_by_type<PhysicsBody>(listener_entity->get_id());

				for (std::shared_ptr<AudioSource>& audio_source: scene->get_components_by_type<AudioSource>())
				{
					AudioSource::PlaybackState playback_state = audio_source->get_playback_state();

					if (playback_state == AudioSource::PlaybackState::PLAYING)
					{
						std::shared_ptr<Entity> source_entity = scene->get_entity(audio_source->get_entity_id());
						std::shared_ptr<PhysicsBody> source_physics_body = scene->get_component_by_type<PhysicsBody>(audio_source->get_entity_id());
						std::shared_ptr<Transform> source_global_transform = scene->calculate_global_transform(source_entity->get_id());

						std::shared_ptr<Audio> audio = audio_source->get_active_audio();
						const int audio_sample_rate = audio->sample_rate;
						const int audio_channel_count = audio->get_channel_count();
						const float gain = audio_source->get_volume() * audio_listener->get_volume();
						const int current_sample_index = (int)(audio_source->playback_time * audio_sample_rate) * audio_channel_count;
						const double duration_per_sample = 1.0 / audio_sample_rate;
						const int total_audio_samples = audio->data.size();

						if (audio_channel_count == 1)
						{
							//Writing mono input to a stereo output
							for (int i = 0; i < (mix_samples_per_frame / 2); ++i)
							{
								int16_t mix_value = 0;

								if (current_sample_index + i < total_audio_samples)
								{
									mix_value = (int16_t)(audio->data[current_sample_index + i] * gain);
								}
								else if (audio_source->is_looping)
								{
									int looped_index = (current_sample_index + i) % total_audio_samples;
									mix_value = (int16_t)(audio->data[looped_index] * gain);
								}

								temp_cumulative_buffer[i * 2] += (float)mix_value;
								temp_cumulative_buffer[i * 2 + 1] += (float)mix_value;
							}
						}
						else if (audio_channel_count == 2)
						{
							//Writing stereo input to stereo output
							for (int i = 0; i < mix_samples_per_frame; ++i)
							{
								int16_t mix_value = 0;

								if (current_sample_index + i < total_audio_samples)
								{
									mix_value = (int16_t)(audio->data[current_sample_index + i] * gain);
								}
								else if (audio_source->is_looping)
								{
									int looped_index = (current_sample_index + i) % total_audio_samples;
									mix_value = (int16_t)(audio->data[looped_index] * gain);
								}

								temp_cumulative_buffer[i] += (float)mix_value;
							}
						}

						if (audio_source->is_capturing_waveform)
						{
							const float source_waveform_capture_quality = audio_source->waveform_capture_sample_count > 0 && 
								audio_source->waveform_capture_sample_count < this->mix_samples_per_frame ? 
								(float)audio_source->waveform_capture_sample_count / (float)this->mix_samples_per_frame : 1.0f;
								
							if (audio_source->current_waveform.size() < this->mix_samples_per_frame * source_waveform_capture_quality)
							{
								audio_source->current_waveform.resize(this->mix_samples_per_frame * source_waveform_capture_quality, 0);
							}

							for (int i = 0; i < audio_source->current_waveform.size(); ++i)
							{
								audio_source->current_waveform[i] = temp_cumulative_buffer[i * (1.0f / source_waveform_capture_quality)];
							}
						}

						audio_source->playback_time += this->mix_samples_per_channel_per_frame * duration_per_sample;

						if (audio_source->playback_time > audio->playback_length)
						{
							audio_source->playback_time = 0.0;

							if (!audio_source->is_looping)
							{
								audio_source->stop();
							}
						}
					}
					else if (playback_state == AudioSource::PlaybackState::STOPPED)
					{
						audio_source->playback_time = 0.0;
					}
				}

				const int half_max_possible_value = std::pow(2, 8 * this->bytes_per_sample) / 2;

				for (int i = 0; i < mix_samples_per_frame; ++i)
				{
					this->mix_buffer[i] = (int16_t)std::clamp(temp_cumulative_buffer[i], (float)-half_max_possible_value, (float)half_max_possible_value);
				}

				if (audio_listener->is_capturing_waveform)
				{
					const float listener_waveform_capture_quality = audio_listener->waveform_capture_sample_count > 0 && 
						audio_listener->waveform_capture_sample_count < this->mix_samples_per_frame ? 
						(float)audio_listener->waveform_capture_sample_count / (float)this->mix_samples_per_frame : 1.0f;

					if (audio_listener->current_waveform.size() < mix_samples_per_frame * listener_waveform_capture_quality)
					{
						audio_listener->current_waveform.resize(this->mix_samples_per_frame * listener_waveform_capture_quality, 0);
					}

					for (int i = 0; i < audio_listener->current_waveform.size(); ++i)
					{
						audio_listener->current_waveform[i] = (float)mix_buffer[i * (1.0f / listener_waveform_capture_quality)];
					}
				}
			}
		}

		SDL_PutAudioStreamData(
            this->audio_stream, 
            this->mix_buffer.data(), 
            (int)(this->mix_buffer.size() * this->bytes_per_sample)
        );
	}

	frame_time_clock->set_end();
}

void Omnific::AudioSystem::finalize()
{
	if (this->is_initialized) 
	{
        SDL_UnbindAudioStream(this->audio_stream);
        SDL_DestroyAudioStream(this->audio_stream);
        SDL_CloseAudioDevice(this->device_id);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    this->is_initialized = false;
}