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
#include "scene/assets/asset.hpp"

namespace Omnia
{
	class OMNIA_ENGINE_API Shader : public Asset
	{
	public:
		enum class ShaderType
		{
			VERTEX,
			FRAGMENT
		};

		static constexpr const char* TYPE_STRING = "Shader";
		Shader() 
		{ 
			this->type = TYPE_STRING;
		};
		Shader(std::string sourceFilepath, ShaderType type);

		virtual Registerable* instance() override
		{
			Shader* clone = new Shader(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		void setSource(std::string source, ShaderType type);
		std::string getSource();
		ShaderType getType();
	private:
		std::string source;
		ShaderType shaderType;
	};
}