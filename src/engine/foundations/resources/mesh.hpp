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

#include "foundations/resources/resource.hpp"
#include <glm.hpp>
#include <vector>
#include <string>
#include <stdint.h>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Mesh : public Resource
	{
	public:
		enum class PrimitiveMode
		{
			TRIANGLES,
			POINTS,
			LINE_STRIP
		};

		Mesh() 
		{ 
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Mesh";

		Mesh(std::string filepath);
		Mesh(std::vector<float> positions,
			PrimitiveMode primitive_mode);
		Mesh(std::vector<float> positions,
			std::vector<float> texture_coords);
		Mesh(std::vector<float> positions,
			std::vector<float> texture_coords,
			std::vector<float> normals);
		Mesh(std::vector<float> positions,
			std::vector<float> texture_coords,
			std::vector<uint32_t> indices);
		Mesh(std::vector<float> positions,
			std::vector<float> texture_coords,
			std::vector<float> normals,
			std::vector<uint32_t> indices);
		Mesh(std::vector<float> positions,
			std::vector<float> texture_coords,
			std::vector<float> normals,
			std::vector<float> tangents,
			std::vector<float> bitangents,
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

		virtual Registerable* instance() override
		{
			Mesh* clone = new Mesh(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		bool get_is_indexed();
		PrimitiveMode get_primitive_mode();

	private:
		bool is_indexed = true;
		PrimitiveMode primitive_mode = PrimitiveMode::TRIANGLES;

		void set_to_cube();
		void set_to_quad();

		void populate_data(
			std::vector<float> positions,
			std::vector<float> texture_coords,
			std::vector<float> normals,
			std::vector<float> tangents,
			std::vector<float> bitangents,
			std::vector<uint32_t> indices);
	};
}