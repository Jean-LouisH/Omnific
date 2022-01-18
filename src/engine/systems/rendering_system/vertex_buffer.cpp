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

#include "vertex_buffer.hpp"

Esi::VertexBuffer::VertexBuffer()
{

}

Esi::VertexBuffer::VertexBuffer(std::shared_ptr<Mesh> mesh, std::shared_ptr<VertexArray> vertexArray)
{
	vertexArray->bind();
}

Esi::VertexBuffer::VertexBuffer(std::shared_ptr<Image> image, std::shared_ptr<VertexArray> vertexArray)
{
	float meshData[] = {
		 // positions         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	this->indexCount = sizeof(indices)/sizeof(unsigned int);
	vertexArray->bind();
	
	glGenBuffers(1, &this->vertexBufferID);
	glGenBuffers(1, &this->elementBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData), meshData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

Esi::VertexBuffer::~VertexBuffer()
{
	this->deleteVertexBuffer();
}

void Esi::VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
}

void Esi::VertexBuffer::deleteVertexBuffer()
{
	glDeleteBuffers(1, &this->vertexBufferID);
	glDeleteBuffers(1, &this->elementBufferID);
}

unsigned int Esi::VertexBuffer::getIndexCount()
{
	return this->indexCount;
}

void Esi::VertexBuffer::buffer(float* meshData, unsigned int* indices)
{
	glGenBuffers(1, &this->vertexBufferID);
	glGenBuffers(1, &this->elementBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData), meshData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}