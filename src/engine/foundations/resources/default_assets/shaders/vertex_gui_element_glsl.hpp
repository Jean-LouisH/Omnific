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
        const char vertex_gui_element_glsl[] = R"(
            #version 330 core
            layout (location = 0) in vec3 model_vertex_translation;
            layout (location = 2) in vec2 model_vertex_uv;
            out vec3 translation;
            out vec2 uv;
            uniform vec2 gui_element_position;
            uniform vec2 screen_viewport;
        
            void main()
            {
                translation = model_vertex_translation;
                uv = vec2(model_vertex_uv.x, model_vertex_uv.y);
        
                float x = ((gui_element_position.x + translation.x) - (screen_viewport.x / 2.0)) / (screen_viewport.x / 2.0);
                float y = ((gui_element_position.y + translation.y) - (screen_viewport.y / 2.0)) / (screen_viewport.y / 2.0);
                float z_rotation = 0.0;
        
                gl_Position = vec4(
                                    x * cos(z_rotation) - y * sin(z_rotation), 
                                    x * sin(z_rotation) + y * cos(z_rotation), 
                                    0.0, 
                                    1.0);
            }	
        )";
    }
}