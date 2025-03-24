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

#pragma once

#include <vector>
#include <string>
#include <scene/components/audio_source.hpp>
#include <memory>
#include <al.h>

namespace Omnific
{
	class OpenALSource
	{
	public:

		OpenALSource();
		~OpenALSource();
		OpenALSource(std::shared_ptr<AudioSource> audio_source);
		void attach_buffer(ALuint buffer_id);
		void set_pitch(float value);
		void set_gain(float value);
		void set_max_distance(float value);
		void set_rolloff_factor(float value);
		void set_reference_distance(float value);
		void set_min_gain(float value);
		void set_max_gain(float value);
		void set_cone_outer_gain(float value);
		void set_cone_inner_angle(float value);
		void set_cone_outer_angle(float value);
		void set_is_relative_to_source(bool value);
		void set_is_looping(bool value);
		void set_position(glm::vec3 position);
		void set_velocity(glm::vec3 linear_velocity);
		void set_direction(glm::vec3 direction);
		void set_playback_position_by_seconds(float value);
		void set_playback_position_by_samples(int value);
		void set_playback_position_by_bytes(int value);
		void play();
		void pause();
		void stop();
		void rewind();
		void queue_buffers(std::vector<ALuint> buffers);
		void unqueue_buffers(std::vector<ALuint> buffers);
		void get_source_type(int* type);
		void get_source_state(int* state);
		void get_buffers_queued_count(int* count);
		void get_buffers_processed_count(int* count);
		float get_playback_position();
		bool is_playing();

		void delete_source();
	private:
		ALuint source_id = 0;
		ALuint attached_buffer_id = 0;
	};
}

