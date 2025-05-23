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
        const char gui_element_glsl[] = R"(
            #version 330 core
            in vec2 uv;
            out vec4 colour;
            uniform vec4 highlight_colour;
            uniform float alpha;
            uniform sampler2D albedo_texture_sampler;
            uniform bool is_highlightable;
            uniform bool is_xstretched_to_panel;
		    uniform bool is_ystretched_to_panel;
            uniform bool is_hovered_in_focus;
		    uniform bool is_selected;

            void main()
            {    
                colour = texture(albedo_texture_sampler, uv);
                colour = mix(colour, highlight_colour, highlight_colour.a);
                colour.a *= alpha;
            }  
        )";
    }
}