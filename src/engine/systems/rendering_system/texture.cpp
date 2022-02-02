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

#include "texture.hpp"

Esi::Texture::Texture()
{

}

Esi::Texture::~Texture()
{
	this->deleteTexture();
}

Esi::Texture::Texture(std::shared_ptr<Image> image)
{
	float borderColour[] = { 1.0, 1.0, 0.0, 0.0 };
	glGenTextures(1, &this->textureID);
	this->type = "image";
	this->textureUnit = 0;
	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image != nullptr)
	{
		uint64_t format = 0;
		uint64_t internalFormat = 0;

		switch (image->getBytesPerPixel())
		{
			case 1: 
				format = GL_RED; 
				internalFormat = GL_RED;
				break;
			case 3: 
				format = GL_RGB; 
				internalFormat = GL_RGBA;
				break;
			case 4: 
				format = GL_RGBA; 
				internalFormat = GL_RGBA;
				break;
		}

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			internalFormat,
			image->getWidth(),
			image->getHeight(),
			0,
			format,
			GL_UNSIGNED_BYTE,
			image->getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Esi::Texture::activateDefaultTextureUnit()
{
	glActiveTexture(GL_TEXTURE0);
}

void Esi::Texture::activateTextureUnit()
{
	glActiveTexture(GL_TEXTURE0 + this->textureUnit);
}

void Esi::Texture::bind()
{
	this->activateTextureUnit();
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Esi::Texture::deleteTexture()
{
	glDeleteTextures(1, &this->textureID);
}