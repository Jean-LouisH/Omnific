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
#include <SDL.h>
#include <math.h>
#include <algorithm>
#include <samplerate.h>
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
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
	}
	else
	{
		this->is_initialized = true;
		SDL_AudioSpec desired = {0};
		SDL_AudioSpec obtained;
		desired.freq = this->mix_sample_frequency;
		desired.format = AUDIO_S16SYS;
		desired.channels = this->mix_channel_count;
		desired.samples = this->mix_samples_per_channel_per_frame * this->mix_channel_count * this->bytes_per_sample;
		desired.callback = NULL;

		this->device_id = SDL_OpenAudioDevice(
			NULL,
			0,
			&desired,
			&obtained,
			NULL);

		SDL_PauseAudioDevice(this->device_id, 0);
	}

	Profiler::add_clock(AUDIO_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"audio_system", "on_output_frame_time"});
	Platform::get_logger().write("Initialized Audio System.");
}

void Omnific::AudioSystem::on_output(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(AUDIO_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	if (EventBus::has_event(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED))
	{
		SDL_ClearQueuedAudio(this->device_id);
	}

	const int queue_refill_threshold = this->mix_samples_per_frame * this->bytes_per_sample * 8;
	uint32_t queued_audio_size = SDL_GetQueuedAudioSize(this->device_id);

	if (queued_audio_size < queue_refill_threshold)
	{
		std::map<SceneLayerID, std::shared_ptr<SceneLayer>>& scene_layers = scene->get_scene_layers();
		std::vector<float> temp_cumulative_buffer(this->mix_samples_per_frame, 0.0f);

		for (auto& [id, scene_layer] : scene_layers)
		{
			for (std::shared_ptr<AudioListener>& audio_listener : scene_layer->get_components_by_type<AudioListener>())
			{
				std::shared_ptr<Entity> listener_entity = scene_layer->get_entity(audio_listener->get_entity_id());
				std::shared_ptr<Transform> listener_global_transform = scene_layer->calculate_global_transform(listener_entity->get_id());
				std::shared_ptr<PhysicsBody> listener_physics_body = scene_layer->get_component_by_type<PhysicsBody>(listener_entity->get_id());

				if (scene_layer->is_2d)
					listener_global_transform->flatten_to_2d();

				for (std::shared_ptr<AudioSource>& audio_source: scene_layer->get_components_by_type<AudioSource>())
				{
					std::shared_ptr<Entity> source_entity = scene_layer->get_entity(audio_source->get_entity_id());
					std::shared_ptr<PhysicsBody> source_physics_body = scene_layer->get_component_by_type<PhysicsBody>(audio_source->get_entity_id());
					std::shared_ptr<Transform> source_global_transform = scene_layer->calculate_global_transform(source_entity->get_id());

					if (scene_layer->is_2d)
						source_global_transform->flatten_to_2d();

					std::shared_ptr<Audio> audio = audio_source->get_active_audio();
					if (audio->sample_rate != this->mix_sample_frequency)
						this->resample_and_replace_audio(audio_source);
					const int audio_sample_rate = audio->sample_rate;
					const int audio_channel_count = audio->get_channel_count();
					const float gain = audio_source->get_volume() * audio_listener->get_volume();
					const int current_sample_index = (int)(audio_source->playback_time * audio_sample_rate) * audio_channel_count;
					const double duration_per_sample = 1.0 / audio_sample_rate;
					const int total_audio_samples = audio->data.size();
					AudioSource::PlaybackState playback_state = audio_source->get_playback_state();

					if (playback_state == AudioSource::PlaybackState::PLAYING)
					{
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
			}
		}

		const int half_max_possible_value = std::pow(2, 8 * this->bytes_per_sample) / 2;

		for (int i = 0; i < mix_samples_per_frame; ++i)
		{
			this->mix_buffer[i] = (int16_t)std::clamp(temp_cumulative_buffer[i], (float)-half_max_possible_value, (float)half_max_possible_value);
		}

		SDL_QueueAudio(this->device_id, this->mix_buffer.data(), this->mix_buffer.size() * this->bytes_per_sample);
	}

	frame_time_clock->set_end();
}

void Omnific::AudioSystem::finalize()
{
	if (this->is_initialized)
	{
		SDL_PauseAudioDevice(this->device_id, 1);
		SDL_CloseAudioDevice(this->device_id);
		SDL_CloseAudio();
	}

	this->is_initialized = false;
}

void Omnific::AudioSystem::resample_and_replace_audio(std::shared_ptr<Omnific::AudioSource> audio_source)
{
	Logger& logger = Platform::get_logger();
	std::shared_ptr<Audio> audio = audio_source->get_active_audio();
	size_t input_size = audio->data.size();
	int channel_count = audio->channel_count;
	std::vector<float> input_float(input_size);
    std::vector<float> output_float;

	const int half_max_possible_value = std::pow(2, 8 * this->bytes_per_sample) / 2;

	logger.write("Audio asset '" + audio->get_name() + "' has an input sample rate of " + 
	std::to_string(audio->sample_rate) + " vs the output sample rate of " + std::to_string(this->mix_sample_frequency) + 
	". Resampling in progress... " 
	);

    // Convert int16_t to float
    for (size_t i = 0; i < input_size; ++i)
        input_float[i] = audio->data[i] / (float)half_max_possible_value;

    double ratio = static_cast<double>(this->mix_sample_frequency) / audio->sample_rate;
    size_t output_length = static_cast<size_t>(input_float.size() * ratio);
    output_float.resize(output_length + 4096);

    SRC_DATA src_data;
    src_data.data_in = input_float.data();
    src_data.input_frames = input_size / channel_count;
    src_data.data_out = output_float.data();
    src_data.output_frames = output_float.size() / channel_count;
    src_data.src_ratio = ratio;
    src_data.end_of_input = 1;

    int error = src_simple(&src_data, SRC_SINC_FASTEST, channel_count);
    if (error) {
        std::cerr << "Resampling error: " << src_strerror(error) << "\n";
        return;
    }

    // Convert back to int16_t
    std::vector<int16_t> output_pcm(src_data.output_frames_gen * channel_count);
    for (size_t i = 0; i < output_pcm.size(); ++i)
        output_pcm[i] = std::clamp(static_cast<int>(output_float[i] * half_max_possible_value), -half_max_possible_value, half_max_possible_value);

	
	std::shared_ptr<Audio> resampled_audio(new Audio(output_pcm, channel_count, this->mix_sample_frequency, output_pcm.size() / channel_count));
	audio_source->remove_audio(audio->get_name());
	audio_source->add_audio(resampled_audio);
	logger.write("'" + audio->get_name() + "' resampled.");
}