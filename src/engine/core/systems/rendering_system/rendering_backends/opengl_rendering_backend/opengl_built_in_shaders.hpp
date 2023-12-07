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

namespace Omnia
{
	namespace OpenGLBuiltInShaders
	{
		namespace Vertex
		{
			const char dimension_2[] = R"(
				//2D vertex shader

				#version 330 core
				layout (location = 0) in vec3 model_vertex_translation;
				layout (location = 2) in vec2 model_vertex_uv;
				out vec3 translation;
				out vec2 uv;
				uniform vec3 entity_translation;
				uniform vec3 entity_rotation;
				uniform vec3 entity_scale;
				uniform vec2 camera_viewport;
				uniform vec3 camera_translation;
				uniform vec3 camera_rotation;

				void main()
				{
					translation = model_vertex_translation;
					uv = vec2(model_vertex_uv.x, model_vertex_uv.y);

					float x = ((entity_translation.x + (translation.x * entity_scale.x)) - camera_translation.x) / (camera_viewport.x / 2.0);
					float y = ((entity_translation.y + (translation.y * entity_scale.y)) - camera_translation.y) / (camera_viewport.y / 2.0);
					float z_rotation = entity_rotation.z - camera_rotation.z;

					gl_Position = vec4(
										x * cos(z_rotation) - y * sin(z_rotation), 
										x * sin(z_rotation) + y * cos(z_rotation), 
										0.0, 
										1.0);
				}	
			)";

			const char dimension_3[] = R"(
				//3D vertex shader

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

		namespace Fragment
		{
			const char dimension_2[] = R"(
				//2D fragment shader

				#version 330 core
				in vec2 uv;
				out vec4 colour;
				uniform float alpha;
				uniform sampler2D albedo_texture_sampler;
				void main()
				{    
					colour = texture(albedo_texture_sampler, uv);
					colour.a *= alpha;
				}  
			)";

			const char dimension_3[] = R"(
				//3D fragment shader

				#version 330 core
				in vec3 translation;
				in vec2 uv;
				in vec3 normal;
				in vec3 fragment_translation;
				out vec4 colour;
				uniform vec4 light_colours[1];
				uniform vec3 light_translations[1];
				uniform vec3 camera_translation;
				uniform float alpha;
				uniform sampler2D albedo_texture_sampler;
				uniform sampler2D metallicity_texture_sampler;
				uniform sampler2D roughness_texture_sampler;
				uniform sampler2D emission_texture_sampler;
				uniform sampler2D normal_texture_sampler;

				void main()
				{    
					colour = texture(albedo_texture_sampler, uv);
					colour.a *= alpha;
				}  
			)";
		}
	}
}