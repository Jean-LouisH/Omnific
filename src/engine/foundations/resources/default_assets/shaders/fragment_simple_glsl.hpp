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
        const char fragment_simple_glsl[] = R"(
            #version 330 core
            
            #define LIGHT_DIRECTIONAL 0
            #define LIGHT_POINT 1
            #define LIGHT_SPOT 2
            #define MAX_LIGHTS 8

            #define PI 3.1415926

            in vec3 translation;
            in vec2 uv;
            in vec3 normal;
            in vec3 fragment_translation;
            out vec4 colour;
            uniform vec4 highlight_colour;
            uniform int light_count;
            uniform int light_modes[MAX_LIGHTS];
            uniform vec3 light_colours[MAX_LIGHTS];
            uniform float light_intensities[MAX_LIGHTS];
            uniform float light_ranges[MAX_LIGHTS];
            uniform vec3 light_translations[MAX_LIGHTS];
            uniform vec3 light_rotations[MAX_LIGHTS];
            uniform float light_inner_cutoff_angles[MAX_LIGHTS];
		    uniform float light_outer_cutoff_angles[MAX_LIGHTS];
            uniform vec3 camera_translation;
            uniform float alpha;
            uniform sampler2D albedo_texture_sampler;


            void main()
            {
                vec3 albedo = texture(albedo_texture_sampler, uv).rgb;
                float shininess = 16;
                vec3 ambient = vec3(0.2) * albedo;
                vec3 reflected_light = vec3(0.0);

                for (int i = 0; i < light_count && i < MAX_LIGHTS; ++i)
                {
                    int light_mode = light_modes[i];
                    float light_intensity = light_intensities[i];
                    vec3 light_translation = light_translations[i];
                    vec3 light_colour = light_colours[i];
                    vec3 light_rotation = light_rotations[i];
                    float light_range = light_ranges[i];
                    vec3 normal_direction = normalize(normal);
                    vec3 light_direction = vec3(0.0);
                    vec3 light_radiance = vec3(0.0);

                    if (light_mode == LIGHT_DIRECTIONAL)
                    {
                        light_direction = vec3(
                                            cos(light_rotation.x) * sin(light_rotation.y), 
                                            -sin(light_rotation.x),
                                            cos(light_rotation.x) * cos(light_rotation.y)
                                            );

                        light_radiance = light_colour * light_intensity;
                    }
                    else if (light_mode == LIGHT_POINT || light_mode == LIGHT_SPOT)
                    {
                        light_direction = normalize(light_translation - fragment_translation);
                        float light_distance = length(light_translation - fragment_translation);
                        float attenuation = 1.0 / (1.0 + (light_distance / light_range) * (light_distance / light_range));
                        light_radiance = light_colour * light_intensity * attenuation;

                        if (light_mode == LIGHT_SPOT)
                        {            
                            vec3 spot_direction = vec3(
                                                        cos(light_rotation.y) * cos(light_rotation.x), 
                                                        sin(light_rotation.y) * cos(light_rotation.x),
                                                        sin(light_rotation.x)
                                                    );
                            float theta = dot(light_direction, normalize(spot_direction));
                            float intensity = clamp((theta - cos(light_outer_cutoff_angles[i])) / (cos(light_inner_cutoff_angles[i]) - cos(light_outer_cutoff_angles[i])), 0.0, 1.0);
                            light_radiance *= intensity;
                        }
                    }

                    vec3 view_direction = normalize(camera_translation - fragment_translation);
                    vec3 reflection_direction = reflect(-light_direction, normal_direction);
                    float r_dot_v = max(dot(view_direction, reflection_direction), 0.0);
                    float n_dot_l = max(dot(normal_direction, light_direction), 0.0);
                    
                    // Lambert
                    vec3 diffuse_component = albedo / PI * n_dot_l;

                    //Phong
                    vec3 specular_component = vec3(max(pow(r_dot_v, shininess), 0.0));

                    reflected_light += (diffuse_component + specular_component) * light_radiance;
                }
                colour = vec4(ambient + reflected_light, 1.0);
                colour = mix(colour, highlight_colour, highlight_colour.a);
                colour.a *= alpha;
            }
        )";
    }
}