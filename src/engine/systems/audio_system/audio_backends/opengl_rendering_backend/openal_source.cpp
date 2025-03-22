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

Omnific::OpenALSource::OpenALSource()
{

}

Omnific::OpenALSource::~OpenALSource()
{
	this->delete_source();
}

Omnific::OpenALSource::OpenALSource(std::shared_ptr<AudioSource> audio_source)
{
	// glGenTextures(1, &this->texture_id);
	// this->bind();

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// std::shared_ptr<Image> texture_image;

	// if (image == nullptr)
	// 	texture_image = std::shared_ptr<Image>(new Image("Image::default"));
	// else
	// 	texture_image = image;

	// uint64_t format = 0;
	// uint64_t internal_format = 0;

	// switch (texture_image->get_bytes_per_pixel())
	// {
	// 	case 1: 
	// 		format = GL_RGBA8;
	// 		break;
	// 	case 3: 
	// 		format = GL_RGB; 
	// 		break;
	// 	case 4: 
	// 		format = GL_RGBA; 
	// 		break;
	// }

	// glTexImage2D(
	// 	GL_TEXTURE_2D,
	// 	0,
	// 	GL_RGBA,
	// 	texture_image->get_width(),
	// 	texture_image->get_height(),
	// 	0,
	// 	format,
	// 	GL_UNSIGNED_BYTE,
	// 	texture_image->get_data());
	// glGenerateMipmap(GL_TEXTURE_2D);
}

void Omnific::OpenALSource::bind()
{
	// this->activate_default_texture_unit();
	// glBindTexture(GL_TEXTURE_2D, this->texture_id);
}

void Omnific::OpenALSource::delete_source()
{
	//glDeleteTextures(1, &this->texture_id);
}