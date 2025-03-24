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

#include "openal_listener.hpp"
#include <al.h>
#include <gtc/type_ptr.hpp>

Omnific::OpenALListener::OpenALListener()
{

}

Omnific::OpenALListener::OpenALListener(std::shared_ptr<AudioListener> audio_listener)
{
	
}

Omnific::OpenALListener::~OpenALListener()
{
	this->delete_listener();
}

void Omnific::OpenALListener::set_gain(float value)
{
	alListenerf(AL_GAIN, value);
}

void Omnific::OpenALListener::set_position(glm::vec3 position)
{
	alListenerfv(AL_POSITION, glm::value_ptr(position));
}

void Omnific::OpenALListener::set_orientation(glm::vec3 up_vector)
{
	alListenerfv(AL_ORIENTATION, glm::value_ptr(up_vector));
}

void Omnific::OpenALListener::set_velocity(glm::vec3 linear_velocity)
{
	alListenerfv(AL_VELOCITY, glm::value_ptr(linear_velocity));
}

void Omnific::OpenALListener::delete_listener()
{

}