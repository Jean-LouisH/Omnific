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

Omnia::Shader::Shader(std::string shader_preset)
{
	this->type = TYPE_STRING;
	this->shader_preset = shader_preset;
}

Omnia::Shader::Shader(std::string vertex_source_input, std::string fragment_source_input, bool is_vertex_filepath, bool is_fragment_filepath)
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

std::string Omnia::Shader::get_preset()
{
	return this->shader_preset;
}

std::string Omnia::Shader::get_vertex_source()
{
	return this->vertex_source;
}

std::string Omnia::Shader::get_fragment_source()
{
	return this->fragment_source;
}