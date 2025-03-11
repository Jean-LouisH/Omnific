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

#include <string>
#include "foundations/resources/resource.hpp"
#include <unordered_map>
#include <glm.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Shader : public Resource
	{
	public:

		static constexpr const char* TYPE_STRING = "Shader";
		Shader() 
		{ 
			this->type = TYPE_STRING;
		};
		Shader(std::string shader_preset);
		Shader(std::string vertex_source_input, std::string fragment_source_input, bool is_vertex_filepath = true, bool is_fragment_filepath = true);

		virtual Registerable* instance() override
		{
			Shader* clone = new Shader(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::string get_preset();
		std::string get_vertex_source();
		std::string get_fragment_source();
	private:
		std::string shader_preset;
		std::string vertex_source;
		std::string fragment_source;
	};

	class OMNIFIC_ENGINE_API ShaderParameters
	{
	public:
		void set_int_uniform(std::string uniform_name, int value);
		void set_bool_uniform(std::string uniform_name, bool value);
		void set_float_uniform(std::string uniform_name, float value);
		void set_vec2_uniform(std::string uniform_name, glm::vec2 value);
		void set_vec3_uniform(std::string uniform_name, glm::vec3 value);
		void set_vec4_uniform(std::string uniform_name, glm::vec4 value);
		void set_mat4_uniform(std::string uniform_name, glm::mat4 value);

		std::unordered_map<std::string, int> int_uniforms;
		std::unordered_map<std::string, bool> bool_uniforms;
		std::unordered_map<std::string, float> float_uniforms;
		std::unordered_map<std::string, glm::vec2> vec2_uniforms;
		std::unordered_map<std::string, glm::vec3> vec3_uniforms;
		std::unordered_map<std::string, glm::vec4> vec4_uniforms;
		std::unordered_map<std::string, glm::mat4> mat4_uniforms;
	private:

	};
}