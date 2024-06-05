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

#include "model.hpp"
#include <foundations/singletons/platform/platform.hpp>


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

void Omnia::Model::deserialize(YAML::Node yaml_node)
{
	this->material = std::shared_ptr<Material>(new Material());
	this->material->set_to_default();

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "mesh")
		{
			this->mesh = std::shared_ptr<Mesh>(new Mesh(it3->second.as<std::string>()));
		}
		else if (it3->first.as<std::string>() == "albedo")
		{
			this->material->albedo = std::shared_ptr<Image>(new Image(it3->second.as<std::string>()));
		}
		else if (it3->first.as<std::string>() == "shader" || it3->first.as<std::string>() == "overriding_shader")
		{
			std::string vertex = "";
			std::string fragment = "";
			std::string preset = "Shader::UNLIT";

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
				else if (it4->first.as<std::string>() == "preset")
				{
					preset = it4->second.as<std::string>();
				}
			}

			std::shared_ptr<Shader> shader;

			if (preset == "")
				shader = std::shared_ptr<Shader>(new Shader(vertex, fragment));
			else
				shader = std::shared_ptr<Shader>(new Shader(preset));

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

void Omnia::Model::set_to_cube()
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = std::shared_ptr<Material>(new Material());
	this->material->set_to_default();
	this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
}

void Omnia::Model::set_to_textured_cube(std::shared_ptr<Material> material)
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = material;
}

void Omnia::Model::set_to_image(std::shared_ptr<Image> image)
{
	this->material = std::shared_ptr<Material>(new Material());
	//this->material->set_to_default();
	this->material->albedo = image;
	glm::vec2 dimensions = image->get_dimensions();
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
	int width = dimensions.x;
	int height = dimensions.y;
	int x_centre = width / 2;
	int y_centre = height / 2;

	if (image->get_alignment() == Image::Alignment::CENTRE)
	{
		x_centre = width / 2;
		y_centre = height / 2;
	}
	else if (image->get_alignment() == Image::Alignment::TOP_LEFT)
	{
		x_centre = 0;
		y_centre = 0;
	}
	else if (image->get_alignment() == Image::Alignment::TOP_RIGHT)
	{
		x_centre = width;
		y_centre = 0;
	}

	/* This stretches the mesh dimensions to the model. */
	mesh->vertices[0].position = glm::vec3(width - x_centre, height - y_centre, 0.0); //top right
	mesh->vertices[1].position = glm::vec3(width - x_centre, 0 - y_centre, 0.0); //bottom right
	mesh->vertices[2].position = glm::vec3(0 - x_centre, 0 - y_centre, 0.0); //bottom left
	mesh->vertices[3].position = glm::vec3(0 - x_centre, height - y_centre, 0.0); //top left
}

void Omnia::Model::set_dimensions(float width, float height)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
}

void Omnia::Model::set_dimensions(float width, float height, float depth)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
	this->dimensions.z = depth;
}

void Omnia::Model::set_shader(std::shared_ptr<Shader> shader)
{
	if (this->overriding_shader == nullptr)
		this->build_uniform_references_from_shader(shader);
	this->shader = shader;
}

void Omnia::Model::set_overriding_shader(std::shared_ptr<Shader> overriding_shader)
{
	this->build_uniform_references_from_shader(overriding_shader);
	this->overriding_shader = overriding_shader;
}

void Omnia::Model::set_alpha(uint8_t value)
{
	this->alpha = value;
}

void Omnia::Model::set_face_culling_to_none()
{
	this->cull_mode = FaceCullMode::NONE;
}

void Omnia::Model::set_face_culling_to_front()
{
	this->cull_mode = FaceCullMode::FRONT;
}

void Omnia::Model::set_face_culling_to_back()
{
	this->cull_mode = FaceCullMode::BACK;
}

void Omnia::Model::set_face_culling_to_front_and_back()
{
	this->cull_mode = FaceCullMode::FRONT_AND_BACK;
}

uint8_t Omnia::Model::get_alpha()
{
	return this->alpha;
}

float Omnia::Model::get_alpha_in_percentage()
{
	return (this->alpha / 255.0);
}

bool Omnia::Model::is_no_face_culling()
{
	return this->cull_mode == FaceCullMode::NONE;
}

bool Omnia::Model::is_front_face_culling()
{
	return this->cull_mode == FaceCullMode::FRONT;
}

bool Omnia::Model::is_back_face_culling()
{
	return this->cull_mode == FaceCullMode::BACK;
}

bool Omnia::Model::is_front_and_back_face_culling()
{
	return this->cull_mode == FaceCullMode::FRONT_AND_BACK;
}

void Omnia::Model::hide()
{
	this->alpha = 0;
}

void Omnia::Model::show()
{
	this->alpha = 255;
}

Omnia::Model::FaceCullMode Omnia::Model::get_face_cull_mode()
{
	return this->cull_mode;
}

std::shared_ptr<Omnia::Image> Omnia::Model::get_image()
{
	return this->material->albedo;
}

std::shared_ptr<Omnia::Shader> Omnia::Model::get_shader()
{
	return this->shader;
}

std::shared_ptr<Omnia::Shader> Omnia::Model::get_overriding_shader()
{
	return this->overriding_shader;
}

glm::vec3 Omnia::Model::get_dimensions()
{
	return this->dimensions;
}

bool Omnia::Model::is_renderable()
{
	return true;
}

void Omnia::Model::build_uniform_references_from_shader(std::shared_ptr<Shader> shader)
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