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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Omnific::ShaderProgram::ShaderProgram(GLuint programID)
{
	this->programID = programID;
}

Omnific::ShaderProgram::~ShaderProgram()
{
	this->deleteProgram();
}

void Omnific::ShaderProgram::use()
{
	if (this->programID != 0)
		glUseProgram(this->programID);
}

void Omnific::ShaderProgram::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnific::ShaderProgram::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Omnific::ShaderProgram::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnific::ShaderProgram::setMat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Omnific::ShaderProgram::logUniforms()
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

void Omnific::ShaderProgram::deleteProgram()
{
	if (this->programID != 0)
		glDeleteProgram(this->programID);
}