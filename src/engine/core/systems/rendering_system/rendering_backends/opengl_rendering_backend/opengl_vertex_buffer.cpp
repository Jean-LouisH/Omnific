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
		if (asset->is_type(Mesh::TYPE_STRING))
		{
			mesh = std::dynamic_pointer_cast<Mesh>(asset);
		}
		else if (asset->is_type(Image::TYPE_STRING))
		{
			std::shared_ptr<Image> image = std::dynamic_pointer_cast<Image>(asset);
			if (image != nullptr)
			{
				glm::vec2 dimensions = image->get_dimensions();
				mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
				int width = dimensions.x;
				int height = dimensions.y;
				int x_centre = width / 2;
				int y_centre = height / 2;


				if (image->get_alignment() == Image::Alignment::CENTRE)
				{
					x_centre = width / 2;
					y_centre = height / 2;
				}
				else if (image->get_alignment() == Image::Alignment::TOP_LEFT)
				{
					x_centre = 0;
					y_centre = 0;
				}
				else if (image->get_alignment() == Image::Alignment::TOP_RIGHT)
				{
					x_centre = width;
					y_centre = 0;
				}

				/* This stretches the mesh dimensions to the renderable component. */
				mesh->vertices[0].position = glm::vec3(width - x_centre, height - y_centre, 0.0); //top right
				mesh->vertices[1].position = glm::vec3(width - x_centre, 0 - y_centre, 0.0); //bottom right
				mesh->vertices[2].position = glm::vec3(0 - x_centre, 0 - y_centre, 0.0); //bottom left
				mesh->vertices[3].position = glm::vec3(0 - x_centre, height - y_centre, 0.0); //top left
			}
		}

		if (mesh != nullptr)
		{
			this->vertex_count = mesh->vertices.size();
			glGenBuffers(1, &this->vertex_buffer_id);
			glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
			glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
		}
	}
}

Omnia::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	this->delete_vertex_buffer();
}

void Omnia::OpenGLVertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
}

void Omnia::OpenGLVertexBuffer::delete_vertex_buffer()
{
	glDeleteBuffers(1, &this->vertex_buffer_id);
}

unsigned int Omnia::OpenGLVertexBuffer::get_vertex_count()
{
	return this->vertex_count;
}