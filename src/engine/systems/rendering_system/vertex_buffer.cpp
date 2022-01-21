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
	this->indexCount = mesh->indices.size();
	vertexArray->bind();

	glGenBuffers(1, &this->vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int),
		&mesh->indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::normal));
	// vertex uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, Mesh::Vertex::uv));

	vertexArray->unbind();
}

Esi::VertexBuffer::VertexBuffer(std::shared_ptr<Image> image, std::shared_ptr<VertexArray> vertexArray)
{
	float meshVertices[] = 
	{
		 // positions         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = 
	{
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	this->indexCount = sizeof(indices)/sizeof(unsigned int);
	vertexArray->bind();
	
	glGenBuffers(1, &this->vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshVertices), meshVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &this->elementBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// uv attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Esi::VertexBuffer::~VertexBuffer()
{
	this->deleteVertexBuffer();
}

void Esi::VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementBufferID);
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