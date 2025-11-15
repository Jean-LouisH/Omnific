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
        const char fragment_pbr_glsl[] = R"(
            #version 330 core

            const int LIGHT_DIRECTIONAL = 0;
            const int LIGHT_POINT = 1;
            const int LIGHT_SPOT = 2;

            const int MAX_LIGHTS = 8;

            const int NONE = 0;
            const int LAMBERT = 1;
            const int BURLEY = 2; 
            const int OREN_NAYER = 3;

            const int PHONG = 1;
            const int BLINN_PHONG = 2;
            const int GGX = 3;
            const int BECKMANN = 4;

            const float PI = 3.14159265;

            in vec3 translation;
            in vec2 uv;
            in vec3 normal;
            in vec3 tangent;
            in vec3 bitangent;
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
            uniform int diffuse_reflection_model;
            uniform int specular_reflection_model;
            uniform sampler2D albedo_texture_sampler;
            uniform sampler2D metallicity_texture_sampler;
            uniform sampler2D roughness_texture_sampler;
            uniform sampler2D emission_texture_sampler;
            uniform sampler2D normal_texture_sampler;

            void main()
            {   
                float shininess = 32;

                vec3 albedo = texture(albedo_texture_sampler, uv).rgb;
                float metalness = clamp(texture(metallicity_texture_sampler, uv).r, 0.0, 1.0);
                float roughness = clamp(texture(roughness_texture_sampler, uv).r, 0.04, 1.0);
                float emission = texture(emission_texture_sampler, uv).r;

                float alpha_r = roughness * roughness; 

                // vec3 n = normalize(normal);
                // vec3 t = normalize(tangent);
                // t = normalize(t - n * dot(n, t));
                // vec3 b = normalize(cross(n, t));
                // mat3 TBN = mat3(t, b, n);
                // vec3 normal_map = texture(normal_texture_sampler, uv).rgb;
                // float epsilon = 0.001;

                // if (length(normal_map) < 0.0 + epsilon)
                //     normal_map = vec3(0.5, 0.5, 1.0);

                // normal_map = normal_map * 2.0 - 1.0;

                // vec3 normal_direction = normalize(TBN * normal_map);
                vec3 normal_direction = normalize(normal);
                
                vec3 emitted_light = vec3(emission);
                vec3 camera_direction = normalize(camera_translation - fragment_translation);
                vec3 reflected_light = vec3(0.0);
                vec3 specular_component = vec3(0.0);
                float diffuse = 0.0;
                vec3 diffuse_component = vec3(0.0);

                vec3 F0 = vec3(0.04);
                F0 = mix(F0, albedo, metalness);
                float n_dot_v = max(dot(normal_direction, camera_direction), 0.0);
                float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
                int loop_count = min(light_count, MAX_LIGHTS);

                for (int i = 0; i < loop_count; ++i)
                {
                    int light_mode = light_modes[i];
                    float light_intensity = light_intensities[i];
                    vec3 light_translation = light_translations[i];
                    vec3 light_colour = light_colours[i];
                    vec3 light_rotation = light_rotations[i];
                    float light_range = light_ranges[i];
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

                    vec3 halfway_vector = normalize(light_direction + camera_direction);
                    float n_dot_h = max(dot(normal_direction, halfway_vector), 0.0);
                    float n_dot_l = max(dot(light_direction, normal_direction), 0.0);
                    float h_dot_v = max(dot(halfway_vector, camera_direction), 0.0);
                    float l_dot_v = max(dot(light_direction, camera_direction), 0.0);
                    float l_dot_h = max(dot(light_direction, halfway_vector), 0.0);

                    vec3 fresnel = vec3(0.0);

                    if (diffuse_reflection_model == LAMBERT)
                    {
                        diffuse = 1.0 / PI;
                    }
                    else if (diffuse_reflection_model == BURLEY)
                    {
                        float Fd90 = 0.5 + 2.0 * l_dot_h * l_dot_h * roughness;
                        float light_scatter = 1.0 + (Fd90 - 1.0) * pow(1.0 - n_dot_l, 5.0);
                        float view_scatter  = 1.0 + (Fd90 - 1.0) * pow(1.0 - n_dot_v, 5.0);
                        diffuse = light_scatter * view_scatter / PI;
                    }
                    else if (diffuse_reflection_model == OREN_NAYER)
                    {
                        float sigma = roughness * roughness;
                        float sigma_2 = sigma * sigma;
                        float A = 1.0 - (sigma_2 / (2.0 * (sigma_2 + 0.33)));
                        float B = 0.45 * sigma_2 / (sigma_2 + 0.09);
                        float s = l_dot_v - n_dot_l * n_dot_v;
                        float t = (s > 0.0) ? s / max(n_dot_l, n_dot_v) : 0.0;
                        diffuse = (A + B * t) / PI;
                    }

                    if (specular_reflection_model == PHONG)
                    {
                        vec3 reflection_direction = reflect(-light_direction, normal_direction);
                        float r_dot_v = max(dot(camera_direction, reflection_direction), 0.0);
                        specular_component = pow(r_dot_v, shininess) * F0;
                    }
                    // Use the Cook-Torrance model anytime these are used
                    else
                    {
                        if (n_dot_l > 0.0 && n_dot_v > 0.0)
                        {
                            float distribution = 0.0;
                            float epsilon = 1e-8; // to mitigate division by zero

                            if (specular_reflection_model == GGX)
                            {
                                float alpha_r_2 = alpha_r * alpha_r;
                                float denominator = (n_dot_h * n_dot_h * (alpha_r_2 - 1.0) + 1.0);
                                distribution = alpha_r_2 / (PI * denominator * denominator + epsilon);
                            }
                            else if (specular_reflection_model == BLINN_PHONG)
                            {
                                distribution = (shininess + 2.0) * pow(n_dot_h, shininess) / (2.0 * PI);
                            }
                            else if (specular_reflection_model == BECKMANN)
                            {
                                float n_dot_h_2 = n_dot_h * n_dot_h;
                                float tan_2 = (1.0 - n_dot_h_2) / n_dot_h_2;
                                float alpha_r_2 = alpha_r * alpha_r;
                                float exponent = -tan_2 / alpha_r_2;
                                distribution = exp(exponent) / (PI * alpha_r_2 * n_dot_h_2 * n_dot_h_2 + epsilon);
                            }

                            // Geometry Smith
                            float geometry = ((n_dot_v) / (n_dot_v * (1.0 - k) + k + epsilon)) * 
                                ((n_dot_l) / (n_dot_l * (1.0 - k) + k) + epsilon);	

                            // Fresnel Schlick
                            fresnel = F0 + (1.0 - F0) * pow(1.0 - h_dot_v, 5.0);

                            specular_component = (distribution * geometry * fresnel) / max(4.0 * n_dot_v * n_dot_l, epsilon);
                        }
                    }
                    		
                    if (specular_reflection_model == PHONG)
                    {
                        fresnel = F0;
                    }

                    diffuse_component = diffuse * albedo * (1 - fresnel) * (1.0 - metalness);
                    reflected_light += (diffuse_component + specular_component) * light_radiance * n_dot_l; 
                }

                vec3 ambient = vec3(0.03) * albedo * (1.0 - metalness);
                vec3 outgoing_light = emitted_light + reflected_light + ambient;
                colour = vec4(outgoing_light, alpha);
                colour.rgb = mix(colour.rgb, highlight_colour.rgb, highlight_colour.a);
            }  
        )";
    }
}