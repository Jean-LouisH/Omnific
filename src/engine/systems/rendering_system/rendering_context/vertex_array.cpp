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

#include "vertex_array.hpp"

Omnia::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &this->vertexArrayID);
}

Omnia::VertexArray::VertexArray(std::shared_ptr<Omnia::Mesh> mesh)
{
	glGenVertexArrays(1, &this->vertexArrayID);

	if (mesh != nullptr)
	{
		this->bind();
		this->vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(mesh));
		this->indexBuffer = std::shared_ptr<IndexBuffer>(new IndexBuffer(mesh));

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
}

Omnia::VertexArray::VertexArray(std::shared_ptr<Omnia::Image> image, glm::vec3 dimensions)
{
	int stride = 5;

	glGenVertexArrays(1, &this->vertexArrayID);
	this->bind();
	this->vertexBuffer = std::shared_ptr<VertexBuffer>(new VertexBuffer(image, dimensions));
	this->indexBuffer = std::shared_ptr<IndexBuffer>(new IndexBuffer(image));

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// uv attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	this->unbind();
}

Omnia::VertexArray::~VertexArray()
{
	this->deleteVertexArray();
}

void Omnia::VertexArray::bind()
{
	glBindVertexArray(this->vertexArrayID);
}

void Omnia::VertexArray::unbind()
{
	glBindVertexArray(0);
}

void Omnia::VertexArray::deleteVertexArray()
{
	glDeleteVertexArrays(1, &this->vertexArrayID);
}

unsigned int Omnia::VertexArray::getIndexCount()
{
	return this->indexBuffer->getIndexCount();
}