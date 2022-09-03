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
#include <scene/assets/shader.hpp>

namespace Omnia
{
	/* The objects that facilitate GPU computation with settable uniform values. */
	class ShaderProgram
	{
	public:
		ShaderProgram(std::vector<Shader> shaders);
		~ShaderProgram();
		void use();
		void setInt(std::string name, int value);
		void setBool(std::string name, bool value);
		void setFloat(std::string name, float value);
		void setMat4(std::string name, glm::mat4 value);
		void logUniforms();
		void deleteProgram();
	private:
		GLuint programID = 0;

		std::vector<GLuint> vertexShaderIDs;
		std::vector<GLuint> fragmentShaderIDs;

		bool compileVertexShader(std::string vertexShaderSource);
		bool compileFragmentShader(std::string fragmentShaderSource);
		void linkShaderProgram();
		bool checkCompileTimeErrors(GLuint ID, GLuint status);
		void deleteShaders();
	};
}

