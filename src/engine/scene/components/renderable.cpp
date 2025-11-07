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

#include "renderable.hpp"
#include <foundations/singletons/platform/platform.hpp>


void Omnific::Renderable::deserialize(YAML::Node yaml_node)
{
	this->material = std::shared_ptr<Material>(new Material());

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "mesh")
		{
			this->mesh = std::shared_ptr<Mesh>(new Mesh(it3->second.as<std::string>()));
		}
		else if (it3->first.as<std::string>() == "material" | it3->first.as<std::string>() == "overriding_material")
		{
			std::shared_ptr<Material> material(new Material());

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "albedo")
				{
					material->albedo_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "metallic")
				{
					material->metallic_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "specular")
				{
					material->specular_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "roughness")
				{
					material->roughness_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "anisotropy")
				{
					material->anisotropic_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "emission")
				{
					material->emission_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "normal")
				{
					material->normal_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "displacement")
				{
					material->displacement_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "occlusion")
				{
					material->occlusion_map = std::shared_ptr<Image>(new Image(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "diffuse_mode")
				{
					if (it4->second.as<std::string>() == "lambert")
					{
						material->diffuse_reflection_model = Material::DiffuseReflectionModel::LAMBERT;
					}
					else if (it4->second.as<std::string>() == "burley")
					{
						material->diffuse_reflection_model = Material::DiffuseReflectionModel::BURLEY;
					}
					else if (it4->second.as<std::string>() == "oren-nayer")
					{
						material->diffuse_reflection_model = Material::DiffuseReflectionModel::OREN_NAYER;
					}
				}
				else if (it4->first.as<std::string>() == "specular_mode")
				{
					if (it4->second.as<std::string>() == "phong")
					{
						material->specular_reflection_model = Material::SpecularReflectionModel::PHONG;
					}
					else if (it4->second.as<std::string>() == "blinn-phong")
					{
						material->specular_reflection_model = Material::SpecularReflectionModel::BLINN_PHONG;
					}
					else if (it4->second.as<std::string>() == "ggx")
					{
						material->specular_reflection_model = Material::SpecularReflectionModel::GGX;
					}
					else if (it4->second.as<std::string>() == "beckmann")
					{
						material->specular_reflection_model = Material::SpecularReflectionModel::BECKMANN;
					}
				}
			}

			if (it3->first.as<std::string>() == "overriding_material")
				this->overriding_material = material;
			else
				this->material = material;
		}
		else if (it3->first.as<std::string>() == "shader" || it3->first.as<std::string>() == "overriding_shader")
		{
			std::string vertex = "";
			std::string fragment = "";
			std::string preset = "Shader::PBR";

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

void Omnific::Renderable::set_to_cube()
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = std::shared_ptr<Material>(new Material());
	this->material->albedo_map = std::shared_ptr<Image>(new Image("Image::default"));
}

void Omnific::Renderable::set_to_textured_cube(std::shared_ptr<Material> material)
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = material;
}

void Omnific::Renderable::set_to_image(std::shared_ptr<Image> image)
{
	this->material = std::shared_ptr<Material>(new Material());
	this->material->albedo_map = image;
	glm::vec2 dimensions = image->get_dimensions();
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::quad"));
	int width = dimensions.x;
	int height = dimensions.y;
	int x_centre = width / 2;
	int y_centre = height / 2;

	/* This stretches the mesh dimensions to the model. */
	mesh->vertices[0].position = glm::vec3(width - x_centre, height - y_centre, 0.0); //top right
	mesh->vertices[1].position = glm::vec3(width - x_centre, 0 - y_centre, 0.0); //bottom right
	mesh->vertices[2].position = glm::vec3(0 - x_centre, 0 - y_centre, 0.0); //bottom left
	mesh->vertices[3].position = glm::vec3(0 - x_centre, height - y_centre, 0.0); //top left
}

void Omnific::Renderable::set_dimensions(float width, float height)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
}

void Omnific::Renderable::set_dimensions(float width, float height, float depth)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
	this->dimensions.z = depth;
}

void Omnific::Renderable::set_shader(std::shared_ptr<Shader> shader)
{
	if (this->overriding_shader == nullptr)
		this->build_uniform_references_from_shader(shader);
	this->shader = shader;
	this->set_default_reflection_models_by_shader_preset(shader->get_preset());
}

void Omnific::Renderable::set_overriding_shader(std::shared_ptr<Shader> overriding_shader)
{
	this->build_uniform_references_from_shader(overriding_shader);
	this->overriding_shader = overriding_shader;
	this->set_default_reflection_models_by_shader_preset(overriding_shader->get_preset());
}

void Omnific::Renderable::set_diffuse_reflection_model(Material::DiffuseReflectionModel diffuse_mode)
{
	if (this->shader->get_preset() != "Shader::SIMPLE")
		this->material->diffuse_reflection_model = diffuse_mode;
}

void Omnific::Renderable::set_specular_reflection_model(Material::SpecularReflectionModel specular_mode)
{
	if (this->shader->get_preset() != "Shader::SIMPLE")
		this->material->specular_reflection_model = specular_mode;
}

void Omnific::Renderable::set_alpha(uint8_t value)
{
	this->alpha = value;
}

void Omnific::Renderable::set_face_culling_to_none()
{
	this->face_cull_mode = FaceCullMode::NONE;
}

void Omnific::Renderable::set_face_culling_to_front()
{
	this->face_cull_mode = FaceCullMode::FRONT;
}

void Omnific::Renderable::set_face_culling_to_back()
{
	this->face_cull_mode = FaceCullMode::BACK;
}

void Omnific::Renderable::set_face_culling_to_front_and_back()
{
	this->face_cull_mode = FaceCullMode::FRONT_AND_BACK;
}

uint8_t Omnific::Renderable::get_alpha()
{
	return this->alpha;
}

float Omnific::Renderable::get_alpha_in_percentage()
{
	return (this->alpha / 255.0);
}

bool Omnific::Renderable::is_no_face_culling()
{
	return this->face_cull_mode == FaceCullMode::NONE;
}

bool Omnific::Renderable::is_front_face_culling()
{
	return this->face_cull_mode == FaceCullMode::FRONT;
}

bool Omnific::Renderable::is_back_face_culling()
{
	return this->face_cull_mode == FaceCullMode::BACK;
}

bool Omnific::Renderable::is_front_and_back_face_culling()
{
	return this->face_cull_mode == FaceCullMode::FRONT_AND_BACK;
}

bool Omnific::Renderable::is_hidden()
{
	return this->alpha == 0;
}

void Omnific::Renderable::hide()
{
	this->alpha = 0;
}

void Omnific::Renderable::show()
{
	this->alpha = 255;
}

Omnific::Renderable::FaceCullMode Omnific::Renderable::get_face_cull_mode()
{
	return this->face_cull_mode;
}

std::shared_ptr<Omnific::Image> Omnific::Renderable::get_image()
{
	return this->material->albedo_map;
}

std::shared_ptr<Omnific::Shader> Omnific::Renderable::get_shader()
{
	return this->shader;
}

std::shared_ptr<Omnific::Shader> Omnific::Renderable::get_overriding_shader()
{
	return this->overriding_shader;
}

std::string Omnific::Renderable::get_reflection_models_as_string()
{
	std::string surface_mode_string;

	switch (this->material->diffuse_reflection_model)
	{
		case Material::DiffuseReflectionModel::LAMBERT: surface_mode_string += "Lambert"; break;
		case Material::DiffuseReflectionModel::BURLEY: surface_mode_string += "Burley"; break;
		case Material::DiffuseReflectionModel::OREN_NAYER: surface_mode_string += "Oren-Nayer"; break;
	}

	surface_mode_string += " + ";

	switch (this->material->specular_reflection_model)
	{
		case Material::SpecularReflectionModel::PHONG: surface_mode_string += "Phong"; break;
		case Material::SpecularReflectionModel::BLINN_PHONG: surface_mode_string += "Blinn-Phong"; break;
		case Material::SpecularReflectionModel::GGX: surface_mode_string += "GGX"; break;
		case Material::SpecularReflectionModel::BECKMANN: surface_mode_string += "Beckmann"; break;
	}

	return surface_mode_string;
}

glm::vec3 Omnific::Renderable::get_dimensions()
{
	return this->dimensions;
}

bool Omnific::Renderable::is_renderable()
{
	return true;
}

void Omnific::Renderable::build_uniform_references_from_shader(std::shared_ptr<Shader> shader)
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

	for (int i = 0; i < concatenated_sources.size(); ++i)
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

void Omnific::Renderable::set_default_reflection_models_by_shader_preset(std::string shader_preset)
{
	if (shader_preset == "Shader::SIMPLE")
	{
		this->material->diffuse_reflection_model = Material::DiffuseReflectionModel::LAMBERT;
		this->material->specular_reflection_model = Material::SpecularReflectionModel::PHONG;
	}
	else if (shader_preset == "Shader::PBR")
	{
		this->material->diffuse_reflection_model = Material::DiffuseReflectionModel::BURLEY;
		this->material->specular_reflection_model = Material::SpecularReflectionModel::GGX;
	}
}