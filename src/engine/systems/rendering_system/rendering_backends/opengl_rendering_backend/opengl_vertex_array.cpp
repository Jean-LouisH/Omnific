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

#include "opengl_vertex_array.hpp"

Omnific::OpenGLVertexArray::OpenGLVertexArray()
{
	glGenVertexArrays(1, &this->vertex_array_id);
}

Omnific::OpenGLVertexArray::OpenGLVertexArray(std::shared_ptr<Mesh> mesh)
{
	glGenVertexArrays(1, &this->vertex_array_id);
	this->bind();
	this->vertex_buffer = std::shared_ptr<OpenGLVertexBuffer>(new OpenGLVertexBuffer(mesh));
	this->index_buffer = std::shared_ptr<OpenGLIndexBuffer>(new OpenGLIndexBuffer(mesh));

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::normal));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::uv));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::bitangent));

	this->unbind();
}

Omnific::OpenGLVertexArray::~OpenGLVertexArray()
{
	this->delete_vertex_array();
}

void Omnific::OpenGLVertexArray::bind()
{
	glBindVertexArray(this->vertex_array_id);
}

void Omnific::OpenGLVertexArray::unbind()
{
	glBindVertexArray(0);
}

void Omnific::OpenGLVertexArray::delete_vertex_array()
{
	glDeleteVertexArrays(1, &this->vertex_array_id);
}

unsigned int Omnific::OpenGLVertexArray::get_index_count()
{
	return this->index_buffer->get_index_count();
}

unsigned int Omnific::OpenGLVertexArray::get_vertex_count()
{
	return this->vertex_buffer->get_vertex_count();
}