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
        const char phong_glsl[] = R"(
            #version 330 core
            
            #define LIGHT_DIRECTIONAL 0
            #define LIGHT_POINT 1
            #define LIGHT_SPOT 2
            #define MAX_LIGHTS 8

            in vec3 translation;
            in vec2 uv;
            in vec3 normal;
            in vec3 fragment_translation;
            out vec4 colour;
            uniform int light_count;
            uniform int light_modes[MAX_LIGHTS];
            uniform vec3 light_colours[MAX_LIGHTS];
            uniform float light_intensities[MAX_LIGHTS];
            uniform float light_ranges[MAX_LIGHTS];
            uniform vec3 light_translations[MAX_LIGHTS];
            uniform vec3 light_rotations[MAX_LIGHTS];
            uniform vec3 camera_translation;
            uniform float alpha;
            uniform sampler2D albedo_texture_sampler;

            void main()
            {
                float specular_intensity = 0.5;
                float shininess = 32;
                float ambient_strength = 0.1;
                vec3 light_translation = light_translations[0];
                vec3 light_colour = light_colours[0];
                vec3 normal_direction = normalize(normal);
                vec3 light_direction = normalize(light_translation - fragment_translation);
                vec3 view_direction = normalize(camera_translation - fragment_translation);
                vec3 reflection_direction = reflect(-light_direction, normal_direction);
                vec3 ambient = ambient_strength * light_colour;
                vec3 diffuse = max(dot(normal_direction, light_direction), 0.0) * light_colour;
                vec3 specular = pow(max(dot(view_direction, reflection_direction), 0.0), shininess) * specular_intensity * light_colour;
                colour = vec4(ambient + diffuse + specular, 1.0) * texture(albedo_texture_sampler, uv);
                colour.a *= alpha;
            }
        )";
    }
}