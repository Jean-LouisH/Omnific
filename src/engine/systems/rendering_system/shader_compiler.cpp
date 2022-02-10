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

std::shared_ptr<Omnific::ShaderProgram> Omnific::ShaderCompiler::compile(std::vector<Shader> shaders)
{
	std::shared_ptr<Omnific::ShaderProgram> shaderProgram;
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
		shaderProgram = this->linkShaderProgram();

	this->deleteShaders();
	return shaderProgram;
}

bool Omnific::ShaderCompiler::compileVertexShader(std::string vertexShaderSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLchar* source = (GLchar*)vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	this->vertexShaderIDs.push_back(vertexShader);
	return this->checkCompileTimeErrors(vertexShader, GL_COMPILE_STATUS);
}

bool Omnific::ShaderCompiler::compileFragmentShader(std::string fragmentShaderSource)
{
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* source = (GLchar*)fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);
	this->fragmentShaderIDs.push_back(fragmentShader);
	return this->checkCompileTimeErrors(fragmentShader, GL_COMPILE_STATUS);
}

std::shared_ptr<Omnific::ShaderProgram> Omnific::ShaderCompiler::linkShaderProgram()
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
	std::shared_ptr<Omnific::ShaderProgram> shaderProgram = std::shared_ptr<Omnific::ShaderProgram>(new ShaderProgram(programID));
	shaderProgram->logUniforms();
	return shaderProgram;
}

bool Omnific::ShaderCompiler::checkCompileTimeErrors(GLuint ID, GLuint status)
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

void Omnific::ShaderCompiler::deleteShaders()
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