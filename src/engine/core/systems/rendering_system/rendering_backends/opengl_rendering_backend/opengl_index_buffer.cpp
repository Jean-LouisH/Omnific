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

#include "opengl_index_buffer.hpp"
#include <core/components/model.hpp>

Omnia::OpenGLIndexBuffer::OpenGLIndexBuffer()
{

}

Omnia::OpenGLIndexBuffer::OpenGLIndexBuffer(std::shared_ptr<Asset> asset)
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
			mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
		}

		if (mesh != nullptr)
		{
			if (mesh->getIsIndexed())
			{
				this->indexCount = mesh->indices.size();
				glGenBuffers(1, &this->indexBufferID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint32_t),
					&mesh->indices[0], GL_STATIC_DRAW);
			}
		}
	}
}

Omnia::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	this->deleteIndexBuffer();
}

void Omnia::OpenGLIndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
}

void Omnia::OpenGLIndexBuffer::deleteIndexBuffer()
{
	glDeleteBuffers(1, &this->indexBufferID);
}

unsigned int Omnia::OpenGLIndexBuffer::getIndexCount()
{
	return this->indexCount;
}