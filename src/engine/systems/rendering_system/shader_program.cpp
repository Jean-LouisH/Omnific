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

#include "shader_program.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <os/os.hpp>

Esi::ShaderProgram::ShaderProgram(GLuint programID)
{
	this->programID = programID;
}

Esi::ShaderProgram::~ShaderProgram()
{
	this->deleteProgram();
}

void Esi::ShaderProgram::use()
{
	if (this->programID != 0)
		glUseProgram(this->programID);
}

void Esi::ShaderProgram::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Esi::ShaderProgram::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Esi::ShaderProgram::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Esi::ShaderProgram::logUniforms()
{
	int how_many{};
	int bufsize{}; // max name size
	glGetProgramiv(this->programID, GL_ACTIVE_UNIFORMS, &how_many);
	glGetProgramiv(this->programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufsize);

	for (int i{}; i < how_many; ++i)
	{
		GLenum type{};
		int size{};
		std::unique_ptr<char> name(new char[bufsize]);

		glGetActiveUniform(this->programID, i, bufsize, nullptr, &size, &type, name.get());
		OS::getLogger().write("Loaded active uniform: \"" + (std::string)name.get() +
			"\", location: " + std::to_string(glGetUniformLocation(this->programID, name.get())) +
			", type: " + std::to_string(type));
	}
}

void Esi::ShaderProgram::deleteProgram()
{
	if (this->programID != 0)
		glDeleteProgram(this->programID);
}