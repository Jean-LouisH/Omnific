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

#include "renderable_component.hpp"
#include <core/singletons/os/os.hpp>

void Omnia::ShaderParameters::set_int_uniform(std::string uniform_name, int value)
{
	this->int_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_bool_uniform(std::string uniform_name, bool value)
{
	this->bool_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_float_uniform(std::string uniform_name, float value)
{
	this->float_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_vec2_uniform(std::string uniform_name, glm::vec2 value)
{
	this->vec2_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_vec3_uniform(std::string uniform_name, glm::vec3 value)
{
	this->vec3_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_vec4_uniform(std::string uniform_name, glm::vec4 value)
{
	this->vec4_uniforms[uniform_name] = value;
}

void Omnia::ShaderParameters::set_mat4_uniform(std::string uniform_name, glm::mat4 value)
{
	this->mat4_uniforms[uniform_name] = value;
}

void Omnia::RenderableComponent::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "shader" || it3->first.as<std::string>() == "overriding_shader")
		{
			std::string vertex = "";
			std::string fragment = "";

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "vertex")
				{
					vertex = it4->second.as<std::string>();
				}
				else if (it4->first.as<std::string>() == "fragment")
				{
					fragment = it4->second.as<std::string>();
				}
			}

			std::shared_ptr<Shader> shader(new Shader(vertex, fragment));

			if (it3->first.as<std::string>() == "shader")
				this->set_shader(shader);
			else
				this->set_overriding_shader(shader);
		}
		else if (it3->first.as<std::string>() == "dimensions")
		{
			this->set_dimensions(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>());
		}
		else if (it3->first.as<std::string>() == "alpha")
		{
			this->set_alpha((uint8_t)(it3->second.as<double>() * 255.0));
		}
	}
}

void Omnia::RenderableComponent::set_dimensions(float width, float height)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
}

void Omnia::RenderableComponent::set_dimensions(float width, float height, float depth)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
	this->dimensions.z = depth;
}

void Omnia::RenderableComponent::set_shader(std::shared_ptr<Shader> shader)
{
	if (this->overriding_shader == nullptr)
		this->build_uniform_references_from_shader(shader);
	this->shader = shader;
}

void Omnia::RenderableComponent::set_overriding_shader(std::shared_ptr<Shader> overriding_shader)
{
	this->build_uniform_references_from_shader(overriding_shader);
	this->overriding_shader = overriding_shader;
}

void Omnia::RenderableComponent::set_alpha(uint8_t value)
{
	this->alpha = value;
}

void Omnia::RenderableComponent::set_to_no_face_culling()
{
	this->cull_mode = CullMode::NONE;
}

void Omnia::RenderableComponent::set_to_front_face_culling()
{
	this->cull_mode = CullMode::FRONT;
}

void Omnia::RenderableComponent::set_to_back_face_culling()
{
	this->cull_mode = CullMode::BACK;
}

void Omnia::RenderableComponent::set_to_front_and_back_face_culling()
{
	this->cull_mode = CullMode::FRONT_AND_BACK;
}

uint8_t Omnia::RenderableComponent::get_alpha()
{
	return this->alpha;
}

float Omnia::RenderableComponent::get_alpha_in_percentage()
{
	return (this->alpha / 255.0);
}

bool Omnia::RenderableComponent::is_no_face_culling()
{
	return this->cull_mode == CullMode::NONE;
}

bool Omnia::RenderableComponent::is_front_face_culling()
{
	return this->cull_mode == CullMode::FRONT;
}

bool Omnia::RenderableComponent::is_back_face_culling()
{
	return this->cull_mode == CullMode::BACK;
}

bool Omnia::RenderableComponent::is_front_and_back_face_culling()
{
	return this->cull_mode == CullMode::FRONT_AND_BACK;
}

void Omnia::RenderableComponent::hide()
{
	this->alpha = 0;
}

void Omnia::RenderableComponent::show()
{
	this->alpha = 255;
}

Omnia::RenderableComponent::CullMode Omnia::RenderableComponent::get_cull_mode()
{
	return this->cull_mode;
}

std::shared_ptr<Omnia::Image> Omnia::RenderableComponent::get_image()
{
	return this->image;
}

std::shared_ptr<Omnia::Shader> Omnia::RenderableComponent::get_shader()
{
	return this->shader;
}

std::shared_ptr<Omnia::Shader> Omnia::RenderableComponent::get_overriding_shader()
{
	return this->overriding_shader;
}

glm::vec3 Omnia::RenderableComponent::get_dimensions()
{
	return this->dimensions;
}

bool Omnia::RenderableComponent::is_renderable()
{
	return true;
}

void Omnia::RenderableComponent::build_uniform_references_from_shader(std::shared_ptr<Shader> shader)
{
	/* Parse GLSL shader sources for uniform declaration statements */

	enum class ParseState
	{
		READING_KEYWORD,
		READING_TYPE,
		READING_UNIFORM_NAME
	};

	std::string keyword;
	std::string type;
	std::string uniform_name;
	ParseState state = ParseState::READING_KEYWORD;

	std::string concatenated_sources = shader->get_vertex_source() + shader->get_fragment_source();

	for (int i = 0; i < concatenated_sources.size(); i++)
	{
		char c = concatenated_sources[i];

		if (c == ' ')
		{
			if (state == ParseState::READING_KEYWORD && keyword == "uniform")
				state = ParseState::READING_TYPE;
			else if (state == ParseState::READING_TYPE)
				state = ParseState::READING_UNIFORM_NAME;

			keyword = "";
		}
		else if (c == ';')
		{
			if (type == "int")
				this->shader_parameters->int_uniforms.emplace(uniform_name, 0);
			if (type == "bool")
				this->shader_parameters->bool_uniforms.emplace(uniform_name, false);
			if (type == "float")
				this->shader_parameters->float_uniforms.emplace(uniform_name, 0.0);
			if (type == "vec2")
				this->shader_parameters->vec2_uniforms.emplace(uniform_name, glm::vec2());
			if (type == "vec3")
				this->shader_parameters->vec3_uniforms.emplace(uniform_name, glm::vec3());
			if (type == "vec4")
				this->shader_parameters->vec4_uniforms.emplace(uniform_name, glm::vec4());
			if (type == "mat4")
				this->shader_parameters->mat4_uniforms.emplace(uniform_name, glm::mat4());

			keyword = "";
			type = "";
			uniform_name = "";

			state = ParseState::READING_KEYWORD;
		}
		else if (c == '\r' || c == '\n')
		{
			;
		}
		else if (state == ParseState::READING_KEYWORD)
		{
			keyword.push_back(c);
		}
		else if (state == ParseState::READING_TYPE)
		{
			type.push_back(c);
		}
		else if (state == ParseState::READING_UNIFORM_NAME)
		{
			uniform_name.push_back(c);
		}
	}
}