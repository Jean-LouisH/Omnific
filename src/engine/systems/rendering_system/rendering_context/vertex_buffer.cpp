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
#include <scene/components/model.hpp>

Omnia::VertexBuffer::VertexBuffer()
{

}

Omnia::VertexBuffer::VertexBuffer(std::shared_ptr<RenderableComponent> renderableComponent)
{
	std::shared_ptr<Mesh> mesh;

	if (renderableComponent->isType(Model::TYPE_STRING))
	{
		mesh = std::dynamic_pointer_cast<Model>(renderableComponent)->getMesh();
	}
	else
	{
		std::shared_ptr<Image> image = renderableComponent->getImage();
		glm::vec3 dimensions = renderableComponent->getDimensions();
		if (image != nullptr)
		{
			mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
			int width = dimensions.x;
			int height = dimensions.y;
			int xCentre = width / 2;
			int yCentre = height / 2;

			/* This stretches the mesh dimensions to the renderable component. */
			mesh->vertices[0].position = glm::vec3(width - xCentre, height - yCentre, 0.0); //top right
			mesh->vertices[1].position = glm::vec3(width - xCentre, 0 - yCentre, 0.0); //bottom right
			mesh->vertices[2].position = glm::vec3(0 - xCentre, 0 - yCentre, 0.0); //bottom left
			mesh->vertices[3].position = glm::vec3(0 - xCentre, height - yCentre, 0.0); //top left
		}
	}

	if (mesh != nullptr)
	{
		this->vertexCount = mesh->vertices.size();
		glGenBuffers(1, &this->vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
	}
}

Omnia::VertexBuffer::~VertexBuffer()
{
	this->deleteVertexBuffer();
}

void Omnia::VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
}

void Omnia::VertexBuffer::deleteVertexBuffer()
{
	glDeleteBuffers(1, &this->vertexBufferID);
}

unsigned int Omnia::VertexBuffer::getVertexCount()
{
	return this->vertexCount;
}