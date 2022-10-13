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
#include <core/singletons/os/os.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <customization/classes/assets/shader.hpp>
#include <glm/gtc/type_ptr.hpp>

Omnia::ShaderProgram::ShaderProgram(std::vector<Shader> shaders)
{
	bool compilationSuccess = true;
	int shaderCount = shaders.size();

	for (int i = 0; i < shaderCount && compilationSuccess; i++)
	{
		Shader& shader = shaders.at(i);

		switch (shader.getType())
		{
		case Shader::ShaderType::VERTEX:
			compilationSuccess = this->compileVertexShader(shader.getSource());
			break;
		case Shader::ShaderType::FRAGMENT:
			compilationSuccess = this->compileFragmentShader(shader.getSource());
			break;
		}
	}

	if (compilationSuccess)
		this->linkShaderProgram();

	this->deleteShaders();
}

Omnia::ShaderProgram::~ShaderProgram()
{
	this->deleteProgram();
}

void Omnia::ShaderProgram::use()
{
	if (this->programID != 0)
		glUseProgram(this->programID);
}

void Omnia::ShaderProgram::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnia::ShaderProgram::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Omnia::ShaderProgram::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnia::ShaderProgram::setMat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Omnia::ShaderProgram::logUniforms()
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

void Omnia::ShaderProgram::deleteProgram()
{
	if (this->programID != 0)
		glDeleteProgram(this->programID);
}

bool Omnia::ShaderProgram::compileVertexShader(std::string vertexShaderSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar* source = (GLchar*)vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	this->vertexShaderIDs.push_back(vertexShader);
	return this->checkCompileTimeErrors(vertexShader, GL_COMPILE_STATUS);
}

bool Omnia::ShaderProgram::compileFragmentShader(std::string fragmentShaderSource)
{
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* source = (GLchar*)fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);
	this->fragmentShaderIDs.push_back(fragmentShader);
	return this->checkCompileTimeErrors(fragmentShader, GL_COMPILE_STATUS);
}

void Omnia::ShaderProgram::linkShaderProgram()
{
	GLuint programID = glCreateProgram();
	int vertexShaderCount = this->vertexShaderIDs.size();
	for (int i = 0; i < vertexShaderCount; i++)
		glAttachShader(programID, this->vertexShaderIDs.at(i));

	int fragmentShaderCount = this->fragmentShaderIDs.size();
	for (int i = 0; i < fragmentShaderCount; i++)
		glAttachShader(programID, this->fragmentShaderIDs.at(i));

	glLinkProgram(programID);
	this->checkCompileTimeErrors(programID, GL_LINK_STATUS);

	OS::getLogger().write("GLSL shaders compiled successfully under program ID " + std::to_string(programID));
	this->logUniforms();
	this->programID = programID;
}

bool Omnia::ShaderProgram::checkCompileTimeErrors(GLuint ID, GLuint status)
{
	GLint compilationSuccess = GL_FALSE;
	char infoLog[512];

	if (status == GL_COMPILE_STATUS)
		glGetShaderiv(ID, status, &compilationSuccess);
	else if (status == GL_LINK_STATUS)
		glGetProgramiv(ID, status, &compilationSuccess);

	if (!compilationSuccess)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		OS::getLogger().write(infoLog);
	}

	return compilationSuccess;
}

void Omnia::ShaderProgram::deleteShaders()
{
	int vertexShaderCount = this->vertexShaderIDs.size();
	for (int i = 0; i < vertexShaderCount; i++)
		glDeleteShader(this->vertexShaderIDs.at(i));
	this->vertexShaderIDs.clear();

	int fragmentShaderCount = this->fragmentShaderIDs.size();
	for (int i = 0; i < fragmentShaderCount; i++)
		glDeleteShader(this->fragmentShaderIDs.at(i));
	this->fragmentShaderIDs.clear();
}