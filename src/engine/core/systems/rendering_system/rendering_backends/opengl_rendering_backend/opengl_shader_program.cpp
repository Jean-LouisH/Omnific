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
	bool compilation_success = true;

	compilation_success &= this->compile_vertex_shader(shader->get_vertex_source());
	compilation_success &= this->compile_fragment_shader(shader->get_fragment_source());

	if (compilation_success)
	{
		this->link_shader_program();
		this->shader = shader;
	}

	this->delete_shader_object_code();
}

Omnia::OpenGLShaderProgram::~OpenGLShaderProgram()
{
	this->delete_program();
}

void Omnia::OpenGLShaderProgram::use()
{
	if (this->program_id != 0)
		glUseProgram(this->program_id);
}

void Omnia::OpenGLShaderProgram::set_int(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void Omnia::OpenGLShaderProgram::set_bool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->program_id, name.c_str()), (int)value);
}

void Omnia::OpenGLShaderProgram::set_float(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->program_id, name.c_str()), value);
}

void Omnia::OpenGLShaderProgram::set_vec2(std::string name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(this->program_id, name.c_str()), value.x, value.y);
}

void Omnia::OpenGLShaderProgram::set_vec3(std::string name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(this->program_id, name.c_str()), value.x, value.y, value.z);
}

void Omnia::OpenGLShaderProgram::set_vec4(std::string name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(this->program_id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Omnia::OpenGLShaderProgram::set_mat4(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

/**Disclaimer: modified from the work of the author 'Jtaim'. A Disquis user in the LearnOpenGL
Shader tutorial comment section. Reference: https://learnopengl.com/Getting-started/Shaders#comment-4468935635*/
void Omnia::OpenGLShaderProgram::log_uniforms()
{
	int how_many{};
	int bufsize{}; // max name size
	glGetProgramiv(this->program_id, GL_ACTIVE_UNIFORMS, &how_many);
	glGetProgramiv(this->program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufsize);

	for (int i{}; i < how_many; ++i)
	{
		GLenum type{};
		int size{};
		std::unique_ptr<char> name(new char[bufsize]);

		glGetActiveUniform(this->program_id, i, bufsize, nullptr, &size, &type, name.get());
		OS::get_logger().write("Loaded active uniform: \"" + (std::string)name.get() +
			"\", location: " + std::to_string(glGetUniformLocation(this->program_id, name.get())) +
			", type: " + std::to_string(type));
	}
}

void Omnia::OpenGLShaderProgram::delete_program()
{
	if (this->program_id != 0)
		glDeleteProgram(this->program_id);
}

bool Omnia::OpenGLShaderProgram::compile_vertex_shader(std::string vertex_shader_source)
{
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLchar* source = (GLchar*)vertex_shader_source.c_str();
	glShaderSource(vertex_shader_id, 1, &source, NULL);
	glCompileShader(vertex_shader_id);
	this->vertex_shader_id = vertex_shader_id;
	return this->check_compile_time_errors(vertex_shader_id, GL_COMPILE_STATUS);
}

bool Omnia::OpenGLShaderProgram::compile_fragment_shader(std::string fragment_shader_source)
{
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* source = (GLchar*)fragment_shader_source.c_str();
	glShaderSource(fragment_shader_id, 1, &source, NULL);
	glCompileShader(fragment_shader_id);
	this->fragment_shader_id = fragment_shader_id;
	return this->check_compile_time_errors(fragment_shader_id, GL_COMPILE_STATUS);
}

void Omnia::OpenGLShaderProgram::link_shader_program()
{
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, this->vertex_shader_id);
	glAttachShader(program_id, this->fragment_shader_id);
	glLinkProgram(program_id);
	this->check_compile_time_errors(program_id, GL_LINK_STATUS);
	//OS::get_logger().write("GLSL shaders compiled successfully under program ID " + std::to_string(program_id));
	//this->log_uniforms();
	this->program_id = program_id;
}

bool Omnia::OpenGLShaderProgram::check_compile_time_errors(GLuint ID, GLuint status)
{
	GLint compilation_success = GL_FALSE;
	char info_log[512];

	if (status == GL_COMPILE_STATUS)
		glGetShaderiv(ID, status, &compilation_success);
	else if (status == GL_LINK_STATUS)
		glGetProgramiv(ID, status, &compilation_success);

	if (!compilation_success)
	{
		glGetShaderInfoLog(ID, 512, NULL, info_log);
		OS::get_logger().write(info_log);
	}

	return compilation_success;
}

void Omnia::OpenGLShaderProgram::delete_shader_object_code()
{
	glDeleteShader(this->vertex_shader_id);
	glDeleteShader(this->fragment_shader_id);
}