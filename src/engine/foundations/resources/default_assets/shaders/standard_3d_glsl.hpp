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

namespace Omnific
{
    namespace DefaultAssets
    {
        const char standard_3d_glsl[] = R"(
            #version 330 core
            layout (location = 0) in vec3 model_vertex_translation;
            layout (location = 1) in vec3 model_normal;
            layout (location = 2) in vec2 model_vertex_uv;
            layout (location = 3) in vec3 model_tangent;
            layout (location = 4) in vec3 model_bitangent;
            out vec3 translation;
            out vec3 normal;
            out vec2 uv;
            out vec3 tangent;
            out vec3 bitangent;
            out vec3 fragment_translation;
            uniform mat4 mvp;
            uniform mat4 model_to_world_matrix;
            uniform mat4 world_to_model_matrix;

            void main()
            {
                translation = model_vertex_translation;
                normal = mat3(transpose(world_to_model_matrix)) * model_normal;
                uv = model_vertex_uv;
                tangent = model_tangent;
                bitangent = model_bitangent;
                fragment_translation = vec3(model_to_world_matrix * vec4(translation, 1.0));
                gl_Position = mvp *	vec4(translation, 1.0);
            }	
        )";
    }
}