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
        const char pbr_glsl[] = R"(
            #version 330 core

            #define LIGHT_DIRECTIONAL 0
            #define LIGHT_POINT 1
            #define LIGHT_SPOT 2

            #define MAX_LIGHTS 8

            #define PI 3.1415926

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
            uniform sampler2D albedo_texture_sampler;
            uniform sampler2D metallicity_texture_sampler;
            uniform sampler2D roughness_texture_sampler;
            uniform sampler2D emission_texture_sampler;
            uniform sampler2D normal_texture_sampler;


            vec3 get_light_direction(int light_mode, vec3 light_translation, vec3 light_rotation, vec3 fragment_translation)
            {
                vec3 light_direction = vec3(0.0);

                if (light_mode == LIGHT_DIRECTIONAL)
                {
                    light_direction = vec3(
                                        cos(light_rotation.y) * cos(light_rotation.x), 
                                        sin(light_rotation.y) * cos(light_rotation.x),
                                        sin(light_rotation.x)
                                        );
                }
                else if (light_mode == LIGHT_POINT || light_mode == LIGHT_SPOT)
                {
                    light_direction = normalize(light_translation - fragment_translation);
                }

                return light_direction;
            }

            vec3 get_light_radiance(
                int light_mode, 
                vec3 light_colour, 
                float light_intensity, 
                float light_range, 
                vec3 light_translation, 
                vec3 light_rotation, 
                vec3 fragment_translation, 
                float light_inner_cutoff_angle,
                float light_outer_cutoff_angle
            )
            {
                vec3 light_radiance = vec3(0.0);
                float light_distance = length(light_translation - fragment_translation);

                if (light_mode == LIGHT_DIRECTIONAL)
                {
                    light_radiance = light_colour * light_intensity;
                }
                else if (light_mode == LIGHT_POINT || light_mode == LIGHT_SPOT)
                {
                    float attenuation = 1.0 / (1.0 + (light_distance / light_range) * (light_distance / light_range));
                    light_radiance = light_colour.rgb * light_intensity * attenuation;

                    if (light_mode == LIGHT_SPOT)
                    {
                        vec3 light_direction = get_light_direction(
                                        light_mode,
                                        light_translation, 
                                        light_rotation,	
                                        fragment_translation);
                                        
                        vec3 spot_direction = vec3(
                                                    cos(light_rotation.y) * cos(light_rotation.x), 
                                                    sin(light_rotation.y) * cos(light_rotation.x),
                                                    sin(light_rotation.x)
                                                );
                        float theta = dot(light_direction, normalize(spot_direction));
                        float epsilon = 0.1;
                        float intensity = clamp((theta - cos(light_outer_cutoff_angle)) / (cos(light_inner_cutoff_angle) - cos(light_outer_cutoff_angle)), 0.0, 1.0);
                        light_radiance *= intensity;
                    }
                }

                return light_radiance;
            }

            float distribution_ggx(vec3 normal, vec3 halfway_vector, float roughness)
            {
                float n_dot_h = max(dot(normal, halfway_vector), 0.0);
                float denominator = (n_dot_h * n_dot_h * (roughness * roughness - 1.0) + 1.0);
                return (roughness * roughness) / (PI * denominator * denominator);
            }
        
            float geometry_schlick_ggx(vec3 normal, vec3 direction, float k)
            {
                float n_dot_direction = max(dot(normal, direction), 0.0);	
                return (n_dot_direction) / (n_dot_direction * (1.0 - k) + k);
            }

            float geometry_smith(vec3 normal, vec3 camera_direction, vec3 light_direction, float k)
            {
                return geometry_schlick_ggx(normal, camera_direction, k) + geometry_schlick_ggx(normal, light_direction, k);				
            }

            vec3 fresnel_schlick(vec3 halfway_vector, vec3 camera_direction, vec3 F0)
            {
                return F0 + (1.0 - F0) * pow(1.0 - max(dot(halfway_vector, camera_direction), 0.0), 5.0);
            }

            void main()
            {   
                vec3 albedo = texture(albedo_texture_sampler, uv).rgb;
                float metalness = texture(metallicity_texture_sampler, uv).r;
                float roughness = texture(roughness_texture_sampler, uv).r;
                float emission = texture(emission_texture_sampler, uv).r;

                //vec3 tangent_normal = texture(normal_texture_sampler, uv).rgb * 2.0 - 1.0;
                vec3 normal = normalize(normal);

                // vec3 T = normalize(tangent);
                // vec3 B = normalize(bitangent);
                // vec3 N = normalize(normal);
                // mat3 TBN = mat3(T, B, N);
                // vec3 tangent_normal = texture(normal_texture_sampler, uv).rgb * 2.0 - 1.0;
                // vec3 normal = normalize(TBN * tangent_normal);
                vec3 emitted_light = vec3(emission);
                vec3 camera_direction = normalize(camera_translation - fragment_translation);
                vec3 reflected_light = vec3(0.0);

                for (int i = 0; i < light_count && i < MAX_LIGHTS; ++i)
                {
                    vec3 light_direction = get_light_direction(
                                    light_modes[i],
                                    light_translations[i], 
                                    light_rotations[i],	
                                    fragment_translation);

                    vec3 halfway_vector = normalize(light_direction + camera_direction);

                    vec3 light_radiance = get_light_radiance(
                                    light_modes[i], 
                                    light_colours[i], 
                                    light_intensities[i], 
                                    light_ranges[i], 
                                    light_translations[i], 
                                    light_rotations[i], 
                                    fragment_translation,
                                    light_inner_cutoff_angles[i],
                                    light_outer_cutoff_angles[i]);

                    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
                    vec3 F0 = vec3(0.04);
                    F0 = mix(F0, albedo, metalness);
                    float distribution = distribution_ggx(normal, halfway_vector, roughness);
                    float geometry = geometry_smith(normal, camera_direction, light_direction, k);
                    vec3 fresnel = fresnel_schlick(halfway_vector, camera_direction, F0);
                    vec3 specular = (distribution * geometry * fresnel) / 
                                    (4.0 * max(dot(normal, camera_direction), 0.0) * max(dot(normal, light_direction), 0.0));
                    vec3 k_s = fresnel;
                    vec3 k_d = (1.0 - k_s) * (1.0 - metalness);
                    vec3 brdf = k_d * albedo / PI + specular;						
                    reflected_light += max(brdf * light_radiance * max(dot(light_direction, normal), 0.0), 0.0); 
                }

                vec3 ambient = vec3(0.03) * albedo;
                vec3 outgoing_light = emitted_light + reflected_light + ambient;
                colour = vec4(outgoing_light, alpha);
                colour = mix(colour, highlight_colour, highlight_colour.a);
            }  
        )";
    }
}