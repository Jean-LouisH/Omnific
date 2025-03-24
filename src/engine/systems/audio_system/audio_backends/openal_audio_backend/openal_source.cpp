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

#include "openal_source.hpp"
#include <al.h>
#include <gtc/type_ptr.hpp>

Omnific::OpenALSource::OpenALSource()
{

}

Omnific::OpenALSource::~OpenALSource()
{
	this->delete_source();
}

Omnific::OpenALSource::OpenALSource(std::shared_ptr<AudioSource> audio_source)
{
	if (audio_source != nullptr)
		alGenSources(1, &this->source_id);
}

void Omnific::OpenALSource::attach_buffer(ALuint buffer_id)
{
	if (this->attached_buffer_id != buffer_id)
		alSourcei(this->source_id, AL_BUFFER, buffer_id);
	this->attached_buffer_id = buffer_id;
}

void Omnific::OpenALSource::set_pitch(float value)
{
	alSourcef(this->source_id, AL_PITCH, value);
}


void Omnific::OpenALSource::set_gain(float value)
{
	alSourcef(this->source_id, AL_GAIN, value);
}


void Omnific::OpenALSource::set_max_distance(float value)
{
	alSourcef(this->source_id, AL_MAX_DISTANCE, value);
}


void Omnific::OpenALSource::set_rolloff_factor(float value)
{
	alSourcef(this->source_id, AL_ROLLOFF_FACTOR, value);
}


void Omnific::OpenALSource::set_reference_distance(float value)
{
	alSourcef(this->source_id, AL_REFERENCE_DISTANCE, value);
}


void Omnific::OpenALSource::set_min_gain(float value)
{
	alSourcef(this->source_id, AL_MIN_GAIN, value);
}


void Omnific::OpenALSource::set_max_gain(float value)
{
	alSourcef(this->source_id, AL_MAX_GAIN, value);
}


void Omnific::OpenALSource::set_cone_outer_gain(float value)
{
	alSourcef(this->source_id, AL_CONE_OUTER_ANGLE, value);
}


void Omnific::OpenALSource::set_cone_inner_angle(float value)
{
	alSourcef(this->source_id, AL_CONE_INNER_ANGLE, value);
}


void Omnific::OpenALSource::set_cone_outer_angle(float value)
{
	alSourcef(this->source_id, AL_CONE_OUTER_ANGLE, value);
}


void Omnific::OpenALSource::set_is_relative_to_source(bool value)
{
	alSourcei(this->source_id, AL_SOURCE_RELATIVE, value);
}

void Omnific::OpenALSource::set_is_looping(bool value)
{
	alSourcei(this->source_id, AL_LOOPING, value);
}

void Omnific::OpenALSource::set_position(glm::vec3 position)
{
	alSourcefv(this->source_id, AL_POSITION, glm::value_ptr(position));
}

void Omnific::OpenALSource::set_velocity(glm::vec3 linear_velocity)
{
	alSourcefv(this->source_id, AL_VELOCITY, glm::value_ptr(linear_velocity));
}

void Omnific::OpenALSource::set_direction(glm::vec3 direction)
{
	alSourcefv(this->source_id, AL_DIRECTION, glm::value_ptr(direction));
}

void Omnific::OpenALSource::set_playback_position_by_seconds(float value)
{
	alSourcef(this->source_id, AL_SEC_OFFSET, value);
}


void Omnific::OpenALSource::set_playback_position_by_samples(int value)
{
	alSourcei(this->source_id, AL_SAMPLE_OFFSET, value);
}


void Omnific::OpenALSource::set_playback_position_by_bytes(int value)
{
	alSourcei(this->source_id, AL_BYTE_OFFSET, value);
}

void Omnific::OpenALSource::play()
{
	alSourcePlay(this->source_id);
}

void Omnific::OpenALSource::pause()
{
	alSourcePause(this->source_id);
}

void Omnific::OpenALSource::stop()
{
	alSourceStop(this->source_id);
}

void Omnific::OpenALSource::rewind()
{
	alSourceRewind(this->source_id);
}

void Omnific::OpenALSource::queue_buffers(std::vector<ALuint> buffers)
{
	alSourceQueueBuffers(this->source_id, buffers.size(), buffers.data());
}

void Omnific::OpenALSource::unqueue_buffers(std::vector<ALuint> buffers)
{
	alSourceUnqueueBuffers(this->source_id, buffers.size(), buffers.data());
}


void Omnific::OpenALSource::get_source_type(int* type)
{
	alGetSourcei(this->source_id, AL_SOURCE_TYPE, type);
}


void Omnific::OpenALSource::get_source_state(int* state)
{
	alGetSourcei(this->source_id, AL_PITCH, state);
}


void Omnific::OpenALSource::get_buffers_queued_count(int* count)
{
	alGetSourcei(this->source_id, AL_PITCH, count);
}


void Omnific::OpenALSource::get_buffers_processed_count(int* count)
{
	alGetSourcei(this->source_id, AL_PITCH, count);
}

float Omnific::OpenALSource::get_playback_position()
{
	float playback_position = 0.0;
	alGetSourcef(this->source_id, AL_SEC_OFFSET, &playback_position);
	return playback_position;
}

bool Omnific::OpenALSource::is_playing()
{
	ALint state;
	alGetSourcei(this->source_id, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void Omnific::OpenALSource::delete_source()
{
	alDeleteSources(1, &this->source_id);
}