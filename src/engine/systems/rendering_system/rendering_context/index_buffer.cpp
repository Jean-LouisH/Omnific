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

#include "index_buffer.hpp"

Omnia::IndexBuffer::IndexBuffer()
{

}

Omnia::IndexBuffer::IndexBuffer(std::shared_ptr<Mesh> mesh)
{
	if (mesh != nullptr)
	{
		this->indexCount = mesh->indices.size();

		glGenBuffers(1, &this->indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int),
			&mesh->indices[0], GL_STATIC_DRAW);

	}
}

Omnia::IndexBuffer::IndexBuffer(std::shared_ptr<Image> image)
{
	if (image != nullptr)
	{
		unsigned int indices[] =
		{
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		this->indexCount = sizeof(indices) / sizeof(unsigned int);

		glGenBuffers(1, &this->indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
}

Omnia::IndexBuffer::~IndexBuffer()
{
	this->deleteIndexBuffer();
}

void Omnia::IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
}

void Omnia::IndexBuffer::deleteIndexBuffer()
{
	glDeleteBuffers(1, &this->indexBufferID);
}

unsigned int Omnia::IndexBuffer::getIndexCount()
{
	return this->indexCount;
}