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

namespace Omnia
{
	class OMNIA_ENGINE_API Shader : public Resource
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
}