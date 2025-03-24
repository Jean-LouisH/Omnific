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

#include "openal_buffer.hpp"
#include <al.h>

Omnific::OpenALBuffer::OpenALBuffer()
{

}

Omnific::OpenALBuffer::OpenALBuffer(std::shared_ptr<Audio> audio)
{
	if (audio != nullptr)
	{
		alGenBuffers(1, &this->buffer_id);
		alBufferData(
			this->buffer_id, 
			(audio->get_channel_count() == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, 
			audio->get_spectrum().data(), 
			audio->get_size(), 
			audio->get_sample_rate());
	}
}

ALuint Omnific::OpenALBuffer::get_id()
{
	return this->buffer_id;
}

Omnific::OpenALBuffer::~OpenALBuffer()
{
	this->delete_buffer();
}

void Omnific::OpenALBuffer::delete_buffer()
{
	alDeleteBuffers(1, &this->buffer_id);
}