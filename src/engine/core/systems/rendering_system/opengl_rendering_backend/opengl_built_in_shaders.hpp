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
				#version 330 core
				layout (location = 0) in vec3 modelVertexTranslation;
				layout (location = 2) in vec2 modelVertexUV;
				out vec3 translation;
				out vec2 uv;
				uniform vec2 cameraViewport;
				uniform vec2 cameraPosition;
				uniform float cameraRotation;

				void main()
				{
					uv = vec2(modelVertexUV.x, modelVertexUV.y);

					float x = (modelVertexTranslation.x - cameraPosition.x) / cameraViewport.x;
					float y = (modelVertexTranslation.y - cameraPosition.y) / cameraViewport.y;

					gl_Position = vec4(
										x * cos(cameraRotation) - y * sin(cameraRotation), 
										x * sin(cameraRotation) + y * cos(cameraRotation), 
										0.0, 
										1.0);
				}	
			)";

			const char dimension_3[] = R"(
				#version 330 core
				layout (location = 0) in vec3 modelVertexTranslation;
				layout (location = 1) in vec3 modelNormal;
				layout (location = 2) in vec2 modelVertexUV;
				layout (location = 3) in vec3 modelTangent;
				layout (location = 4) in vec3 modelBitangent;
				out vec3 translation;
				out vec3 normal;
				out vec2 uv;
				out vec3 tangent;
				out vec3 bitangent;
				out vec3 fragmentTranslation;
				uniform mat4 mvp;
				uniform mat4 modelToWorldMatrix;
				uniform mat4 worldToModelMatrix;

				void main()
				{
					translation = modelVertexTranslation;
					normal = mat3(transpose(worldToModelMatrix)) * modelNormal;
					uv = modelVertexUV;
					tangent = modelTangent;
					bitangent = modelBitangent;
					fragmentTranslation = vec3(modelToWorldMatrix * vec4(translation, 1.0));
					gl_Position = mvp *	vec4(translation, 1.0);
				}	
			)";
		}

		namespace Fragment
		{
			const char dimension_2[] = R"(
				#version 330 core
				in vec2 uv;
				out vec4 colour;
				uniform float alpha;
				uniform sampler2D albedoTextureSampler;
				void main()
				{    
					colour = texture(albedoTextureSampler, uv);
					colour.a *= alpha;
				}  
			)";

			const char dimension_3[] = R"(
				#version 330 core
				in vec3 translation;
				in vec2 uv;
				in vec3 normal;
				in vec3 fragmentTranslation;
				out vec4 colour;
				uniform vec4 lightColour;
				uniform vec3 lightTranslation;
				uniform vec3 cameraTranslation;
				uniform float alpha;
				uniform sampler2D albedoTextureSampler;
				uniform sampler2D metallicityTextureSampler;
				uniform sampler2D roughnessTextureSampler;
				uniform sampler2D emissionTextureSampler;
				uniform sampler2D normalTextureSampler;

				void main()
				{    
					colour = texture(albedoTextureSampler, uv);
					colour.a *= alpha;
				}  
			)";
		}
	}
}