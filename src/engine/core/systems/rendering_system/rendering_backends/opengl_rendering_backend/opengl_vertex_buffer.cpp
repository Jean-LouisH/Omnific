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

#include "opengl_vertex_buffer.hpp"
#include <core/components/model.hpp>
#include <core/components/gui.hpp>

Omnia::OpenGLVertexBuffer::OpenGLVertexBuffer()
{

}

Omnia::OpenGLVertexBuffer::OpenGLVertexBuffer(std::shared_ptr<Asset> asset)
{
	std::shared_ptr<Mesh> mesh;

	if (asset != nullptr)
	{
		if (asset->isType(Mesh::TYPE_STRING))
		{
			mesh = std::dynamic_pointer_cast<Mesh>(asset);
		}
		else if (asset->isType(Image::TYPE_STRING))
		{
			std::shared_ptr<Image> image = std::dynamic_pointer_cast<Image>(asset);
			if (image != nullptr)
			{
				glm::vec2 dimensions = image->getDimensions();
				mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
				int width = dimensions.x;
				int height = dimensions.y;
				int xCentre = width / 2;
				int yCentre = height / 2;


				if (image->getAlignment() == Image::Alignment::CENTRE)
				{
					xCentre = width / 2;
					yCentre = height / 2;
				}
				else if (image->getAlignment() == Image::Alignment::TOP_LEFT)
				{
					xCentre = 0;
					yCentre = 0;
				}
				else if (image->getAlignment() == Image::Alignment::TOP_RIGHT)
				{
					xCentre = width;
					yCentre = 0;
				}

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
}

Omnia::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	this->deleteVertexBuffer();
}

void Omnia::OpenGLVertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
}

void Omnia::OpenGLVertexBuffer::deleteVertexBuffer()
{
	glDeleteBuffers(1, &this->vertexBufferID);
}

unsigned int Omnia::OpenGLVertexBuffer::getVertexCount()
{
	return this->vertexCount;
}