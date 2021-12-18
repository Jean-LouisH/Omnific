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

namespace Esi
{
	namespace BuiltInShaders
	{
		namespace Vertex
		{
			const char texture[] = R"(
				#version 330 core
				layout (location = 0) in vec4 vertex;
				out vec2 texCoords;
				uniform mat4 model;
				uniform mat4 projection;
				void main()
				{
					texCoords = vertex.zw;
					gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
				}	
			)";
		}

		namespace Fragment
		{
			const char texture[] = R"(
				#version 330 core
				in vec2 texCoords;
				out vec4 color;
				uniform sampler2D image;
				uniform vec3 spriteColor;
				void main()
				{    
					color = vec4(spriteColor, 1.0) * texture(image, texCoords);
				}  
			)";
		}
	}
}