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

Omnific::VertexBuffer::VertexBuffer()
{

}

Omnific::VertexBuffer::VertexBuffer(std::shared_ptr<Mesh> mesh)
{
	if (mesh != nullptr)
	{
		glGenBuffers(1, &this->vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), &mesh->vertices[0], GL_STATIC_DRAW);
	}
}

Omnific::VertexBuffer::VertexBuffer(std::shared_ptr<Image> image, glm::vec3 dimensions)
{
	if (image != nullptr)
	{
		float meshVertices[] =
		{
			// positions         // texture coords
			0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
		   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
		   -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
		};

		int stride = 5;
		int width = dimensions.x;
		int height = dimensions.y;
		int xCentre = width / 2;
		int yCentre = height / 2;

		/* This stretches the dimensions to the image. */

		//top right
		meshVertices[(stride * 0) + 0] = width - xCentre;
		meshVertices[(stride * 0) + 1] = height - yCentre;
		//bottom right
		meshVertices[(stride * 1) + 0] = width - xCentre;
		meshVertices[(stride * 1) + 1] = 0 - yCentre;
		//bottom left
		meshVertices[(stride * 2) + 0] = 0 - xCentre;
		meshVertices[(stride * 2) + 1] = 0 - yCentre;
		//top left
		meshVertices[(stride * 3) + 0] = 0 - xCentre;
		meshVertices[(stride * 3) + 1] = height - yCentre;

		glGenBuffers(1, &this->vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(meshVertices), meshVertices, GL_STATIC_DRAW);
	}
}

Omnific::VertexBuffer::~VertexBuffer()
{
	this->deleteVertexBuffer();
}

void Omnific::VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
}

void Omnific::VertexBuffer::deleteVertexBuffer()
{
	glDeleteBuffers(1, &this->vertexBufferID);
}