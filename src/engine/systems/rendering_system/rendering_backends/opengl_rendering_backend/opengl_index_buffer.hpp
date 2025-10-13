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

#pragma once

#ifdef _WEB_PLATFORM
    #include <GLES3/gl3.h>
#else
    #include <glad/glad.h>
#endif
#include <glm.hpp>
#include <vector>
#include <string>
#include <scene/components/renderable.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/resources/mesh.hpp>
#include <memory>

namespace Omnific
{
	/* Storage for mesh index data in GPU memory. */
	class OpenGLIndexBuffer
	{
	public:
		OpenGLIndexBuffer();
		OpenGLIndexBuffer(std::shared_ptr<Mesh> mesh);
		~OpenGLIndexBuffer();
		void bind();
		void delete_index_buffer();
		unsigned int get_index_count();
	private:
		GLuint index_buffer_id = 0;
		GLsizei index_count = 0;
	};
}

