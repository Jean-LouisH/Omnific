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

#include "application/scene/asset.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <stdint.h>

namespace Omnia
{
	class Mesh : public Asset
	{
	public:
		Mesh() 
		{ 
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Mesh";

		Mesh(std::string filepath);
		Mesh(std::vector<float> positions,
			std::vector<uint32_t> textureCoords);
		Mesh(std::vector<float> positions,
			std::vector<uint32_t> textureCoords,
			std::vector<uint32_t> indices);

		struct Vertex 
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::vec3 tangent;
			glm::vec3 bitangent;
		};

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		bool getIsIndexed();

	private:
		bool isIndexed = true;

		void setToCube();
		void setToQuad();
		void setToPlane();

		void populateData(
			std::vector<float> vertices,
			std::vector<uint32_t> textureCoords,
			std::vector<uint32_t> indices);
		void populateData(
			std::vector<float> positions,
			std::vector<uint32_t> textureCoords);
	};
}