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

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <application/scene/assets/shader.hpp>

namespace Lilliputian
{
	class ShaderCompiler
	{
	public:
		~ShaderCompiler();
		void compile(std::vector<std::string> vertexShaderSources, std::vector<std::string> fragmentShaderSources);
		void compile(std::vector<Shader> shaders);
		void deleteProgram();
		void use();
		void setInt(std::string name, int value);
		void setBool(std::string name, bool value);
		void setFloat(std::string name, float value);
		void logUniforms();
	private:
		std::vector<GLuint> vertexShaderIDs;
		std::vector<GLuint> fragmentShaderIDs;
		GLuint programID;

		bool isCompiled = false;

		bool compileVertexShader(std::string vertexShaderSource);
		bool compileFragmentShader(std::string fragmentShaderSource);
		void linkShaderProgram();
		bool checkCompileTimeErrors(GLuint ID, GLuint status);
		void deleteShaders();
	};
}

