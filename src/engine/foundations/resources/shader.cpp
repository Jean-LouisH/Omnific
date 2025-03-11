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

#include "shader.hpp"
#include <foundations/singletons/uid_generator.hpp>
#include <foundations/singletons/platform/platform.hpp>


Omnific::Shader::Shader(std::string shader_preset)
{
	this->type = TYPE_STRING;
	this->shader_preset = shader_preset;
}

Omnific::Shader::Shader(std::string vertex_source_input, std::string fragment_source_input, bool is_vertex_filepath, bool is_fragment_filepath)
{
	this->type = TYPE_STRING;
	this->shader_preset = "Shader::CUSTOM";

	if (is_vertex_filepath)
	{
		this->vertex_source = Platform::get_file_access().read_string(vertex_source_input);
	}
	else
	{
		this->vertex_source = vertex_source_input;
	}

	if (is_fragment_filepath)
	{
		this->fragment_source = Platform::get_file_access().read_string(fragment_source_input);
	}
	else
	{
		this->fragment_source = fragment_source_input;
	}

	if (is_vertex_filepath && is_fragment_filepath)
	{
		this->set_name(vertex_source_input + ", " + fragment_source_input);
	}
}

std::string Omnific::Shader::get_preset()
{
	return this->shader_preset;
}

std::string Omnific::Shader::get_vertex_source()
{
	return this->vertex_source;
}

std::string Omnific::Shader::get_fragment_source()
{
	return this->fragment_source;
}

void Omnific::ShaderParameters::set_int_uniform(std::string uniform_name, int value)
{
	this->int_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_bool_uniform(std::string uniform_name, bool value)
{
	this->bool_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_float_uniform(std::string uniform_name, float value)
{
	this->float_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_vec2_uniform(std::string uniform_name, glm::vec2 value)
{
	this->vec2_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_vec3_uniform(std::string uniform_name, glm::vec3 value)
{
	this->vec3_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_vec4_uniform(std::string uniform_name, glm::vec4 value)
{
	this->vec4_uniforms[uniform_name] = value;
}

void Omnific::ShaderParameters::set_mat4_uniform(std::string uniform_name, glm::mat4 value)
{
	this->mat4_uniforms[uniform_name] = value;
}