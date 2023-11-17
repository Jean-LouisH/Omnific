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
#include <memory>
#include <core/assets/shader.hpp>

namespace Omnia
{
	/* The objects that facilitate GPU computation with settable uniform values. */
	class OpenGLShaderProgram
	{
	public:
		OpenGLShaderProgram(std::shared_ptr<Shader> shader);
		~OpenGLShaderProgram();
		void use();
		void set_int(std::string name, int value);
		void set_bool(std::string name, bool value);
		void set_float(std::string name, float value);
		void set_vec2(std::string name, glm::vec2 value);
		void set_vec3(std::string name, glm::vec3 value);
		void set_vec4(std::string name, glm::vec4 value);
		void set_mat4(std::string name, glm::mat4 value);
		void log_uniforms();
		void delete_program();
	private:
		GLuint program_id = 0;

		GLuint vertex_shader_id;
		GLuint fragment_shader_id;

		std::shared_ptr<Shader> shader;

		bool compile_vertex_shader(std::string vertex_shader_source);
		bool compile_fragment_shader(std::string fragment_shader_source);
		void link_shader_program();
		bool check_compile_time_errors(GLuint ID, GLuint status);
		void delete_shader_object_code();
	};
}

