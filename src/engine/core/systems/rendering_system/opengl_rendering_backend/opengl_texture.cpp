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

#include "opengl_texture.hpp"

Omnia::OpenGLTexture::OpenGLTexture()
{

}

Omnia::OpenGLTexture::~OpenGLTexture()
{
	this->deleteTexture();
}

Omnia::OpenGLTexture::OpenGLTexture(std::shared_ptr<Image> image)
{
	float borderColour[] = { 0.0, 0.0, 0.0, 0.0 };
	glGenTextures(1, &this->textureID);
	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::shared_ptr<Image> textureImage;

	if (image == nullptr)
		textureImage = std::shared_ptr<Image>(new Image("Image::default"));
	else
		textureImage = image;

	uint64_t format = 0;
	uint64_t internalFormat = 0;

	switch (textureImage->getBytesPerPixel())
	{
		case 1: 
			internalFormat = GL_RGBA;
			format = GL_BGRA;
			break;
		case 3: 
			internalFormat = GL_RGBA;
			format = GL_RGB; 
			break;
		case 4: 
			internalFormat = GL_RGBA;
			format = GL_RGBA; 
			break;
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		internalFormat,
		textureImage->getWidth(),
		textureImage->getHeight(),
		0,
		format,
		GL_UNSIGNED_BYTE,
		textureImage->getData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Omnia::OpenGLTexture::activateDefaultTextureUnit()
{
	glActiveTexture(GL_TEXTURE0);
}

void Omnia::OpenGLTexture::activateTextureUnit(Unit textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + (uint8_t)textureUnit);
}

void Omnia::OpenGLTexture::bind()
{
	this->activateDefaultTextureUnit();
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Omnia::OpenGLTexture::bind(Unit textureUnit)
{
	this->activateTextureUnit(textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void Omnia::OpenGLTexture::deleteTexture()
{
	glDeleteTextures(1, &this->textureID);
}