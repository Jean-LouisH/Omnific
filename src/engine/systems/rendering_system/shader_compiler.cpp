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

#include "shader_compiler.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <os/os.hpp>

Lilliputian::ShaderCompiler::~ShaderCompiler()
{
	this->deleteProgram();
}

void Lilliputian::ShaderCompiler::compile(std::vector<std::string> vertexShaderSources, std::vector<std::string> fragmentShaderSources)
{
	bool compilationSuccess = true;

	int vertexShaderSourceCount = vertexShaderSources.size();
	for (int i = 0; i < vertexShaderSourceCount && compilationSuccess; i++)
		compilationSuccess = this->compileVertexShader(vertexShaderSources.at(i));

	int fragmentShaderSourceCount = fragmentShaderSources.size();
	for (int i = 0; i < fragmentShaderSourceCount && compilationSuccess; i++)
		compilationSuccess = this->compileFragmentShader(fragmentShaderSources.at(i));

	if (compilationSuccess)
		this->linkShaderProgram();

	this->deleteShaders();
	this->isCompiled = true;
}

void Lilliputian::ShaderCompiler::compile(std::vector<Shader> shaders)
{
	std::vector<std::string> vertexShaderSources;
	std::vector<std::string> fragmentShaderSources;

	for (int i = 0; i < shaders.size(); i++)
	{
		Shader& shader = shaders.at(i);
		Shader::Type type = shader.getType();

		switch (type)
		{
			case Shader::Type::VERTEX: vertexShaderSources.push_back(shader.getSource());
			case Shader::Type::FRAGMENT: fragmentShaderSources.push_back(shader.getSource());
		}
	}

	this->compile(vertexShaderSources, fragmentShaderSources);
}

void Lilliputian::ShaderCompiler::deleteProgram()
{
	if (this->isCompiled)
		glDeleteProgram(this->programID);

	this->isCompiled = false;
}

void Lilliputian::ShaderCompiler::use()
{
	if (this->isCompiled)
		glUseProgram(this->programID);
}

bool Lilliputian::ShaderCompiler::compileVertexShader(std::string vertexShaderSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar* source = (GLchar*)vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	this->vertexShaderIDs.push_back(vertexShader);
	return this->checkCompileTimeErrors(vertexShader, GL_COMPILE_STATUS);
}

bool Lilliputian::ShaderCompiler::compileFragmentShader(std::string fragmentShaderSource)
{
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* source = (GLchar*)fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);
	this->fragmentShaderIDs.push_back(fragmentShader);
	return this->checkCompileTimeErrors(fragmentShader, GL_COMPILE_STATUS);
}

void Lilliputian::ShaderCompiler::linkShaderProgram()
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
	this->programID = programID;

	OS::getLogger().write("GLSL shaders compiled successfully under program ID " + std::to_string(this->programID));
	this->logUniforms();
}

bool Lilliputian::ShaderCompiler::checkCompileTimeErrors(GLuint ID, GLuint status)
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

void Lilliputian::ShaderCompiler::deleteShaders()
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

void Lilliputian::ShaderCompiler::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Lilliputian::ShaderCompiler::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Lilliputian::ShaderCompiler::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL 
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Lilliputian::ShaderCompiler::logUniforms()
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