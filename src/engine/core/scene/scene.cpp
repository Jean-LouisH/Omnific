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

#include "scene.hpp"
#include "core/component.hpp"
#include <core/singletons/os/os.hpp>
#include <yaml-cpp/yaml.h>
#include <customization/class_registry/class_registry.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

Omnia::Scene::Scene()
{
	this->id = UIDGenerator::get_new_uid();
}

Omnia::Scene::Scene(std::string filepath)
{
	this->deserialize_from(filepath);
	this->id = UIDGenerator::get_new_uid();
}

Omnia::Scene::Scene(std::string filepath, std::string name)
{
	this->deserialize_from(filepath, name);
	this->id = UIDGenerator::get_new_uid();
}

void Omnia::Scene::serialize_to(std::string filepath)
{

}

void Omnia::Scene::deserialize_from(std::string filepath)
{
	return this->deserialize_from(filepath, "");
}

void Omnia::Scene::deserialize_from(std::string filepath, std::string name)
{
	if (name == "")
		OS::get_logger().write("Loading all SceneLayers from Scene: \"" + filepath + "\"");
	else
		OS::get_logger().write("Loading SceneLayer: \"" + name + "\" from Scene : \"" + filepath + "\"");


	this->name = filepath;
	const std::string full_filepath = OS::get_file_access().get_data_directory_path() + filepath;

	try
	{
		YAML::Node yaml_node = YAML::LoadFile(full_filepath);

		for (YAML::const_iterator it0 = yaml_node.begin(); it0 != yaml_node.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneLayer")
			{
				std::shared_ptr<SceneLayer> scene_layer(new SceneLayer());
				bool load_this_scene_layer = true;

				/* If the name is an empty string, load all, otherwise search for the name */
				if (name != "")
				{
					load_this_scene_layer = false;

					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							load_this_scene_layer = it1->second.as<std::string>() == name;
							break;
						}
					}
				}

				if (load_this_scene_layer)
				{
					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							scene_layer->name = it1->second.as<std::string>();
						}
						else if (it1->first.as<std::string>() == "spatial_dimension")
						{
							int value = it1->second.as<int>();

							if (value == 2)
								scene_layer->is_2d = true;
							else if (value == 3)
								scene_layer->is_2d = false;
						}
						else if (it1->first.as<std::string>() == "Entity")
						{
							std::shared_ptr<Entity> entity(new Entity());
							scene_layer->add_entity(entity);

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								//Entity attributes
								if (it2->first.as<std::string>() == "name")
								{
									scene_layer->set_entity_name(scene_layer->get_last_entity()->get_id(), it2->second.as<std::string>());
								}
								else if (it2->first.as<std::string>() == "parent")
								{
									scene_layer->get_last_entity()->parent_id = scene_layer->get_entity_by_name(it2->second.as<std::string>())->get_id();
								}
								//Components
								else
								{
									std::shared_ptr<Registerable> registerable = ClassRegistry::query<Component>(it2->first.as<std::string>());

									if (registerable != nullptr)
									{
										std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(std::shared_ptr<Registerable>(registerable->instance()));
										component->deserialize(it2->second);
										scene_layer->add_component_to_last_entity(component);
									}
								}
							}
						}
						/* Recursively load another SceneLayer into this one if the filename
						   is not the same. */
						else if (it1->first.as<std::string>() == "SubSceneLayer")
						{
							std::string sub_scene_filepath;
							std::string sub_scene_layer_name;
							EntityID parent_id = 0;

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								if (it2->first.as<std::string>() == "parent")
								{
									parent_id = scene_layer->get_entity_by_name(it2->second.as<std::string>())->get_id();
								}
								else if (it2->first.as<std::string>() == "name")
								{
									sub_scene_filepath = it2->second[0].as<std::string>();
									sub_scene_layer_name = it2->second[1].as<std::string>();
								}
							}

							std::shared_ptr<SceneLayer> sub_scene_layer;

							if (sub_scene_filepath != filepath)
							{
								/*The last SceneLayer is the only SceneTree, so this just extracts the one specified.*/
								if (sub_scene_layer_name != "")
									sub_scene_layer = Scene(sub_scene_filepath, sub_scene_layer_name).get_last_scene_layer();
								else
									sub_scene_layer = this->load_gltf(sub_scene_filepath);


								/* Only load the SceneLayer if it is the same spatial dimension. */
								if (sub_scene_layer->is_2d == scene_layer->is_2d)
								{
									/* Transfer Entities and their Components */
									std::shared_ptr<Entity> new_root_entity(new Entity());
									new_root_entity->set_name(sub_scene_filepath);
									new_root_entity->parent_id = parent_id;
									scene_layer->add_entity(new_root_entity);
									std::unordered_map<EntityID, std::shared_ptr<Entity>>& sub_scene_entities = sub_scene_layer->get_entities();

									/*Entities without parents are listed before others.*/
									std::vector<std::shared_ptr<Entity>> sorted_entities;


									/*Without parents*/
									for (auto it = sub_scene_entities.begin(); it != sub_scene_entities.end(); it++)
									{
										std::shared_ptr<Entity> sub_scene_entity = it->second;

										if (sub_scene_entity->parent_id == 0)
											sorted_entities.push_back(sub_scene_entity);
									}

									/*With parents*/

									for (auto it = sub_scene_entities.begin(); it != sub_scene_entities.end(); it++)
									{
										std::shared_ptr<Entity> sub_scene_entity = it->second;

										if (sub_scene_entity->parent_id != 0)
											sorted_entities.push_back(sub_scene_entity);
									}


									for (size_t i = 0; i < sorted_entities.size(); i++)
									{
										std::shared_ptr<Entity> sub_scene_entity = sorted_entities[i];

										if (sub_scene_entity->parent_id == 0)
											sub_scene_entity->parent_id = new_root_entity->get_id();

										scene_layer->add_entity(sub_scene_entity);

										std::unordered_map<std::string, ComponentID> sub_scene_entity_component_ids = sub_scene_entity->component_ids;

										for (auto it2 = sub_scene_entity_component_ids.begin(); it2 != sub_scene_entity_component_ids.end(); it2++)
											scene_layer->add_component_to_last_entity(sub_scene_layer->get_component_by_id(it2->second));
									}
								}
							}
						}
					}
				}

				this->add_scene_layer(scene_layer);
			}
		}
	}
	catch (int e)
	{

	}
}

void Omnia::Scene::reload()
{
	this->scene_layers.clear();
	this->last_scene_layer_id = 0;
	this->deserialize_from(this->get_name());
}

void Omnia::Scene::add_scene_layer(std::shared_ptr<SceneLayer> scene_layer)
{
	this->scene_layers.emplace(scene_layer->get_id(), scene_layer);
	this->last_scene_layer_id = scene_layer->get_id();
}

void Omnia::Scene::add_empty_scene_layer()
{
	std::shared_ptr<SceneLayer> scene_layer = std::shared_ptr<SceneLayer>(new SceneLayer());
	this->add_scene_layer(scene_layer);
}

void Omnia::Scene::remove_scene_layer(SceneLayerID scene_layer_id)
{
	if (this->scene_layers.count(scene_layer_id))
		this->scene_layers.erase(scene_layer_id);
}

std::string Omnia::Scene::get_name()
{
	return this->name;
}

std::shared_ptr<Omnia::SceneLayer> Omnia::Scene::get_scene_layer(SceneLayerID scene_layer)
{
	return this->scene_layers.at(scene_layer);
}

std::shared_ptr<Omnia::SceneLayer> Omnia::Scene::get_scene_layer_by_name(std::string name)
{
	std::shared_ptr<SceneLayer> scene_layer = nullptr;

	for (auto it = this->scene_layers.begin(); it != this->scene_layers.end(); it++)
		if (it->second->get_name() == name)
			return it->second;

	return scene_layer;
}

std::shared_ptr<Omnia::SceneLayer> Omnia::Scene::get_last_scene_layer()
{
	return this->scene_layers.at(this->last_scene_layer_id);
}

std::map<Omnia::SceneLayerID, std::shared_ptr<Omnia::SceneLayer>>& Omnia::Scene::get_scene_layers()
{
	return this->scene_layers;
}

Omnia::SceneID Omnia::Scene::get_id()
{
	return this->id;
}

std::shared_ptr<Omnia::SceneLayer> Omnia::Scene::load_gltf(std::string filepath)
{
	std::shared_ptr<SceneLayer> scene_layer(new SceneLayer());

	tinygltf::Model gltf_model;
	tinygltf::TinyGLTF tiny_gltf;
	std::string err;
	std::string warn;

	bool ret = tiny_gltf.LoadBinaryFromFile(
		&gltf_model,
		&err, 
		&warn, 
		OS::get_file_access().get_data_directory_path() + filepath);

	if (!warn.empty())
		printf("Warn: %s\n", warn.c_str());

	if (!err.empty())
		printf("Err: %s\n", err.c_str());

	if (!ret)
	{
		printf("Failed to parse gl_tf\n");
	}
	else
	{
		std::shared_ptr<Entity> gltf_scene_root_entity(new Entity());
		scene_layer->add_entity(gltf_scene_root_entity);

		for (size_t i = 0; i < gltf_model.nodes.size(); i++)
		{
			tinygltf::Node gltf_node = gltf_model.nodes[i];
			int mesh_index = gltf_node.mesh;

			if (mesh_index != -1)
			{
				// GLTF data
				std::vector<float> positions = this->read_gltfprimitive_attribute(gltf_model, "POSITION", mesh_index);
				std::vector<float> texture_coords = this->read_gltfprimitive_attribute(gltf_model, "TEXCOORD_0", mesh_index);
				std::vector<float> normals = this->read_gltfprimitive_attribute(gltf_model, "NORMAL", mesh_index);
				std::vector<uint32_t> indices = this->read_gltfprimitive_indices(gltf_model, mesh_index);

				std::shared_ptr<Mesh> mesh(new Mesh(positions, texture_coords, normals, indices));
				std::shared_ptr<Material> material(new Material());

				std::shared_ptr<Entity> entity(new Entity());
				entity->parent_id = gltf_scene_root_entity->get_id();
				scene_layer->add_entity(entity);
				std::shared_ptr<Transform> transform(new Transform());
				std::shared_ptr<Model> model(new Model());

				int material_index = gltf_model.meshes.at(mesh_index).primitives.at(0).material;

				if (material_index != -1)
				{
					tinygltf::Material gltf_material = gltf_model.materials.at(gltf_model.meshes.at(mesh_index).primitives.at(0).material);
					int base_colour_texture_index = gltf_material.pbrMetallicRoughness.baseColorTexture.index;
					int metallic_rougness_texture_index = gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index;
					int normal_texture_index = gltf_material.normalTexture.index;
					int emissive_texture_index = gltf_material.emissiveTexture.index;
					int occlusion_texture_index = gltf_material.occlusionTexture.index;

					/* Albedo / BaseColour*/
					if (base_colour_texture_index != -1)
					{
						material->albedo = this->read_gltfimage(gltf_model, base_colour_texture_index);
					}
					else
					{
						std::vector<double> base_color_factor = gltf_material.pbrMetallicRoughness.baseColorFactor;

						if (base_color_factor.size() == 3)
						{
							material->albedo = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
								base_color_factor[0],
								base_color_factor[1],
								base_color_factor[2],
								1.0))
							));
						}
						else if (base_color_factor.size() == 4)
						{
							material->albedo = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
								base_color_factor[0],
								base_color_factor[1],
								base_color_factor[2],
								base_color_factor[3]))
							));
						}
					}

					/* Metallicity and Roughness */
					if (metallic_rougness_texture_index != -1)
					{
						material->metallicity = this->read_gltfimage(gltf_model, metallic_rougness_texture_index);
					}
					else
					{
						double metallic_factor = gltf_material.pbrMetallicRoughness.metallicFactor;
						double roughness_factor = gltf_material.pbrMetallicRoughness.roughnessFactor;

						material->metallicity = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							metallic_factor,
							metallic_factor,
							metallic_factor,
							1.0))
						));

						material->roughness = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							roughness_factor,
							roughness_factor,
							roughness_factor,
							1.0))
						));

					}

					/* Normal Map*/
					if (normal_texture_index != -1)
					{
						material->normal = this->read_gltfimage(gltf_model, normal_texture_index);
					}
					else
					{
						material->normal = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							0.5,
							0.5,
							1.0,
							1.0))
						));
					}

					/* Emission */
					if (emissive_texture_index != -1)
					{
						material->emission = this->read_gltfimage(gltf_model, emissive_texture_index);
					}
					else
					{
						std::vector<double> emissive_factor = gltf_material.emissiveFactor;

						material->emission = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							emissive_factor[0],
							emissive_factor[1],
							emissive_factor[2],
							1.0))
						));
					}

					/* Occlusion */
					if (occlusion_texture_index != -1)
					{
						material->occlusion = this->read_gltfimage(gltf_model, occlusion_texture_index);
					}

				}
				else
				{
					material->set_to_default();
				}

				if (gltf_node.translation.size() == 3)
				{
					transform->translation = {
						gltf_node.translation[0],
						gltf_node.translation[1],
						gltf_node.translation[2] };
				}

				if (gltf_node.rotation.size() == 4)
				{
					glm::quat unit_quaternion = glm::quat(
						gltf_node.rotation[3],
						gltf_node.rotation[0],
						gltf_node.rotation[1],
						gltf_node.rotation[2]);

					transform->rotation = glm::eulerAngles(unit_quaternion);
					transform->rotation *= 180.0 / M_PI;
				}

				if (gltf_node.scale.size() == 3)
				{
					transform->scale = {
						gltf_node.scale[0],
						gltf_node.scale[1],
						gltf_node.scale[2] };
				}

				model->material = material;
				model->mesh = mesh;

				scene_layer->add_component_to_last_entity(std::dynamic_pointer_cast<Component>(transform));
				scene_layer->add_component_to_last_entity(std::dynamic_pointer_cast<Component>(model));
			}
		}
	}

	return scene_layer;
}

std::vector<uint8_t> Omnia::Scene::read_gltfbuffer(std::vector<unsigned char> buffer_data, tinygltf::BufferView buffer_view)
{
	std::vector<uint8_t> bytes(
		buffer_data.begin() + buffer_view.byteOffset,
		buffer_data.begin() + buffer_view.byteOffset + buffer_view.byteLength);

	return bytes;
}

std::vector<float> Omnia::Scene::read_gltfprimitive_attribute(tinygltf::Model model, std::string attribute_name, size_t index)
{
	std::vector<float> attribute;
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);

	if (primitive.attributes.count(attribute_name) > 0)
	{
		tinygltf::Accessor accessor = model.accessors.at(primitive.attributes.at(attribute_name));
		tinygltf::BufferView buffer_view = model.bufferViews.at(accessor.bufferView);
		std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
		std::vector<uint8_t> bytes = this->read_gltfbuffer(buffer, buffer_view);
		float* float_byte_data = (float*)bytes.data();
		size_t float_byte_size = bytes.size() / sizeof(float);

		for (size_t i = 0; i < float_byte_size; i++)
			attribute.push_back(float_byte_data[i]);
	}

	return attribute;
}

std::vector<uint32_t> Omnia::Scene::read_gltfprimitive_indices(tinygltf::Model model, size_t index)
{
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);
	tinygltf::Accessor accessor = model.accessors.at(primitive.indices);
	tinygltf::BufferView buffer_view = model.bufferViews.at(accessor.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
	std::vector<uint8_t> index_bytes = this->read_gltfbuffer(buffer, buffer_view);
	std::vector<uint32_t> indices;

	uint16_t* short_index_byte_data = (uint16_t*)index_bytes.data();
	size_t short_index_byte_size = index_bytes.size() / sizeof(uint16_t);

	for (size_t i = 0; i < short_index_byte_size; i++)
		indices.push_back((uint32_t)short_index_byte_data[i]);

	return indices;
}

std::shared_ptr<Omnia::Image> Omnia::Scene::read_gltfimage(tinygltf::Model model, int texture_index)
{
	int image_index = model.textures[texture_index].source;
	tinygltf::Image gltf_image = model.images[image_index];
	tinygltf::BufferView buffer_view = model.bufferViews.at(gltf_image.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
	std::vector<uint8_t> image_file_bytes = this->read_gltfbuffer(buffer, buffer_view);
	int width = 0;
	int height = 0;
	int colour_channels = 0;

	stbi_set_flip_vertically_on_load(0);

	uint8_t* image_data = stbi_load_from_memory(
		image_file_bytes.data(),
		image_file_bytes.size(),
		&width,
		&height,
		&colour_channels,
		0);

	return std::shared_ptr<Image>(new Image(
		image_data,
		width,
		height,
		colour_channels));
}