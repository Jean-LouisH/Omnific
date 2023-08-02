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

#include "opengl_shader_program.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <core/singletons/os/os.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <core/assets/shader.hpp>
#include <glm/gtc/type_ptr.hpp>

Omnia::OpenGLShaderProgram::OpenGLShaderProgram(std::shared_ptr<Shader> shader)
{
	bool compilationSuccess = true;

	compilationSuccess &= this->compileVertexShader(shader->getVertexSource());
	compilationSuccess &= this->compileFragmentShader(shader->getFragmentSource());

	if (compilationSuccess)
	{
		this->linkShaderProgram();
		this->shader = shader;
	}

	this->deleteShaderObjectCode();
}

Omnia::OpenGLShaderProgram::~OpenGLShaderProgram()
{
	this->deleteProgram();
}

void Omnia::OpenGLShaderProgram::use()
{
	if (this->programID != 0)
		glUseProgram(this->programID);
}

void Omnia::OpenGLShaderProgram::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnia::OpenGLShaderProgram::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Omnia::OpenGLShaderProgram::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Omnia::OpenGLShaderProgram::setVec2(std::string name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(this->programID, name.c_str()), value.x, value.y);
}

void Omnia::OpenGLShaderProgram::setVec3(std::string name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(this->programID, name.c_str()), value.x, value.y, value.z);
}

void Omnia::OpenGLShaderProgram::setVec4(std::string name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(this->programID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Omnia::OpenGLShaderProgram::setMat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Omnia::OpenGLShaderProgram::logUniforms()
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

void Omnia::OpenGLShaderProgram::deleteProgram()
{
	if (this->programID != 0)
		glDeleteProgram(this->programID);
}

bool Omnia::OpenGLShaderProgram::compileVertexShader(std::string vertexShaderSource)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLchar* source = (GLchar*)vertexShaderSource.c_str();
	glShaderSource(vertexShaderID, 1, &source, NULL);
	glCompileShader(vertexShaderID);
	this->vertexShaderID = vertexShaderID;
	return this->checkCompileTimeErrors(vertexShaderID, GL_COMPILE_STATUS);
}

bool Omnia::OpenGLShaderProgram::compileFragmentShader(std::string fragmentShaderSource)
{
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* source = (GLchar*)fragmentShaderSource.c_str();
	glShaderSource(fragmentShaderID, 1, &source, NULL);
	glCompileShader(fragmentShaderID);
	this->fragmentShaderID = fragmentShaderID;
	return this->checkCompileTimeErrors(fragmentShaderID, GL_COMPILE_STATUS);
}

void Omnia::OpenGLShaderProgram::linkShaderProgram()
{
	GLuint programID = glCreateProgram();
	glAttachShader(programID, this->vertexShaderID);
	glAttachShader(programID, this->fragmentShaderID);
	glLinkProgram(programID);
	this->checkCompileTimeErrors(programID, GL_LINK_STATUS);
	//OS::getLogger().write("GLSL shaders compiled successfully under program ID " + std::to_string(programID));
	//this->logUniforms();
	this->programID = programID;
}

bool Omnia::OpenGLShaderProgram::checkCompileTimeErrors(GLuint ID, GLuint status)
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

void Omnia::OpenGLShaderProgram::deleteShaderObjectCode()
{
	glDeleteShader(this->vertexShaderID);
	glDeleteShader(this->fragmentShaderID);
}