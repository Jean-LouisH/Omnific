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
	// if (mesh != nullptr)
	// {
	// 	this->vertex_count = mesh->vertices.size();
	// 	glGenBuffers(1, &this->vertex_buffer_id);
	// 	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
	// 	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
	// }
}

Omnific::OpenALBuffer::~OpenALBuffer()
{
	this->delete_buffer();
}

void Omnific::OpenALBuffer::bind()
{
	//glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
}

void Omnific::OpenALBuffer::delete_buffer()
{
	//glDeleteBuffers(1, &this->vertex_buffer_id);
}