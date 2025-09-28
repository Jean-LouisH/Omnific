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
#include "scene/components/component.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <yaml-cpp/yaml.h>
#include <customization/class_registry.hpp>
#include <gtx/quaternion.hpp>
#include <cmath>
#include <foundations/singletons/profiler.hpp>

#include <foundations/singletons/event_bus.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/viewport.hpp>
#include <scene/components/model.hpp>
#include <gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/matrix_decompose.hpp>

#include <foundations/resources/default_assets/scenes/cone_glb.hpp>
#include <foundations/resources/default_assets/scenes/cube_glb.hpp>
#include <foundations/resources/default_assets/scenes/cylinder_glb.hpp>
#include <foundations/resources/default_assets/scenes/icosphere_glb.hpp>
#include <foundations/resources/default_assets/scenes/monkey_glb.hpp>
#include <foundations/resources/default_assets/scenes/plane_glb.hpp>
#include <foundations/resources/default_assets/scenes/sphere_glb.hpp>
#include <foundations/resources/default_assets/scenes/torus_glb.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

Omnific::Scene::Scene()
{
	Scene("");
}

Omnific::Scene::Scene(std::string filepath)
{
	this->id = UIDGenerator::get_new_uid();
	this->name = "Scene (ID:" + std::to_string(this->id) + ")";
	this->deserialize_from(filepath);
	this->fps_monitor_clock = std::make_shared<Clock>();
	this->fps_monitor_clock->set_start();

}

void Omnific::Scene::serialize_to(std::string filepath)
{

}

void Omnific::Scene::deserialize_from(std::string filepath)
{
	this->name = filepath;

	if (filepath != "")
	{
		FileAccess& file_access = Platform::get_file_access();
		Platform::get_logger().write("Loading Scene from: \"" + filepath + "\"");

		const std::string full_filepath = Platform::get_file_access().find_path(filepath);
		const std::string file_extension = file_access.get_file_extension(filepath);

		if (file_extension == "yml" || file_extension == "yaml")
		{
			try
			{
				YAML::Node yaml_node = YAML::LoadFile(full_filepath);

				std::shared_ptr<Entity> viewport_entity = std::shared_ptr<Entity>(new Entity());
				std::shared_ptr<Viewport> viewport = std::shared_ptr<Viewport>(new Viewport());
				this->add_entity(viewport_entity);
				this->set_entity_name(viewport_entity->get_id(), "Viewport");
				this->add_component_to_last_entity(viewport);

				for (YAML::const_iterator it1 = yaml_node.begin(); it1 != yaml_node.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity" || it1->first.as<std::string>() == "Scene")
					{
						std::shared_ptr<Entity> entity(new Entity());
						this->add_entity(entity);
						std::string name;

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							//Entity attributes
							if (it2->first.as<std::string>() == "name")
							{
								name = it2->second.as<std::string>();
								this->set_entity_name(this->get_last_entity()->get_id(), name);
							}
							else if (it2->first.as<std::string>() == "parent")
							{
								this->add_entity_to_parent_entity_by_name(entity, it2->second.as<std::string>());
							}
							else if ((it2->first.as<std::string>() == "transform"))
							{
								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									std::shared_ptr<Transform> transform = entity->get_transform();

									if (it3->first.as<std::string>() == "translation")
									{
										transform->translation.x = it3->second[0].as<double>();
										transform->translation.y = it3->second[1].as<double>();
										transform->translation.z = it3->second[2].as<double>();
									}
									else if (it3->first.as<std::string>() == "rotation")
									{
										transform->rotation.x = it3->second[0].as<double>();
										transform->rotation.y = it3->second[1].as<double>();
										transform->rotation.z = it3->second[2].as<double>();
									}
									else if (it3->first.as<std::string>() == "scale")
									{
										transform->scale.x = it3->second[0].as<double>();
										transform->scale.y = it3->second[1].as<double>();
										transform->scale.z = it3->second[2].as<double>();
									}
								}					
							}
							else
							{
								//Components
								std::shared_ptr<Registerable> registerable = ClassRegistry::query<Component>(it2->first.as<std::string>());

								if (registerable != nullptr)
								{
									std::shared_ptr<Component> component = std::dynamic_pointer_cast<Component>(std::shared_ptr<Registerable>(registerable->instance()));
									component->deserialize(it2->second);
									this->add_component_to_last_entity(component);
								}
							}
						}

						if (it1->first.as<std::string>() == "Scene")
						{
							if (name != filepath)
							{
								this->merge_another_scene_to_parent_entity(std::shared_ptr<Scene>(new Scene(name)), entity->get_id());
							}
						}
					}
				}
			}
			catch (int e)
			{

			}

			std::shared_ptr<Entity> debug_gui_entity = std::make_shared<Entity>();
			std::shared_ptr<Entity> debug_camera_entity = std::make_shared<Entity>();
			std::shared_ptr<GUI> debug_gui = std::make_shared<GUI>();
			std::shared_ptr<Camera> debug_camera = std::make_shared<Camera>();

			debug_gui->hide();
			debug_gui->set_to_label("");
			this->add_entity(debug_camera_entity);
			this->add_component_to_last_entity(debug_camera);
			this->set_entity_name(debug_gui_entity->get_id(), "debug_camera_entity");
			this->add_entity(debug_gui_entity);
			this->add_component_to_last_entity(debug_gui);
			this->set_entity_name(debug_gui_entity->get_id(), "debug_gui_entity");
		}
		else
		{
			this->load_from_gltf(filepath);
		}
	}
}

void Omnific::Scene::add_entity(std::shared_ptr<Entity> entity)
{
	if (entity != nullptr)
	{
		if (entity->parent_id != 0)
			this->entities.at(entity->parent_id)->child_ids.push_back(entity->id);

		if (this->get_entity(entity->id) == nullptr)
		{
			this->start_entities_queue.emplace(entity->id);
			this->entities.emplace(entity->id, entity);
			this->last_entity_id = entity->id;
			this->set_entity_name(entity->id, entity->name);

			if (this->is_active())
			{
				EventBus::publish_event(OMNIFIC_EVENT_ENTITY_ADDED);
				EventBus::publish_event(OMNIFIC_EVENT_SCENE_MODIFIED);
			}
		}
	}
}

void Omnific::Scene::add_empty_entity()
{
	std::shared_ptr<Entity> empty_entity(new Entity());
	this->add_entity(empty_entity);
}

void Omnific::Scene::add_entity_to_parent_entity(std::shared_ptr<Entity> entity, EntityID parent_entity_id)
{
	std::shared_ptr<Entity> parent_entity = this->get_entity(parent_entity_id);

	if (parent_entity)
	{
		entity->parent_id = parent_entity_id;
		this->add_entity(entity);
	}
}

void Omnific::Scene::add_entity_to_parent_entity_by_name(std::shared_ptr<Entity> entity, std::string parent_entity_name)
{
	std::shared_ptr<Entity> parent_entity = this->get_entity_by_name(parent_entity_name);
	if (parent_entity)
		this->add_entity_to_parent_entity(entity, parent_entity->id);
}

void Omnific::Scene::merge_another_scene_to_parent_entity(std::shared_ptr<Scene> other_scene, EntityID parent_entity_id)
{
	if (other_scene != nullptr)
	{
		std::shared_ptr<Entity> viewport_entity = other_scene->get_entity_by_name("Viewport");
		std::shared_ptr<Entity> debug_camera_entity = other_scene->get_entity_by_name("debug_camera_entity");
		std::shared_ptr<Entity> debug_gui_entity = other_scene->get_entity_by_name("debug_gui_entity");
		
		if (viewport_entity != nullptr)
			other_scene->remove_entity(viewport_entity->get_id());
		if (debug_camera_entity != nullptr)
			other_scene->remove_entity(debug_camera_entity->get_id());
		if (debug_gui_entity != nullptr)
			other_scene->remove_entity(debug_gui_entity->get_id());

		/* Transfer Entities and their Components */
		std::unordered_map<EntityID, std::shared_ptr<Entity>>& other_scene_entities = other_scene->get_entities();

		/*Entities without parents are listed before others.*/
		std::vector<std::shared_ptr<Entity>> sorted_entities;

		/*Without parents*/
		for (const auto& [id, other_scene_entity] : other_scene_entities)
			if (other_scene_entity->parent_id == 0)
				sorted_entities.push_back(other_scene_entity);

		/*With parents*/
		for (const auto& [id, other_scene_entity] : other_scene_entities)
			if (other_scene_entity->parent_id != 0)
				sorted_entities.push_back(other_scene_entity);


		for (size_t i = 0; i < sorted_entities.size(); ++i)
		{
			std::shared_ptr<Entity> other_scene_entity = sorted_entities[i];

			other_scene_entity->child_ids.clear();

			if (other_scene_entity->parent_id == 0)
				this->add_entity_to_parent_entity(other_scene_entity, parent_entity_id);
			else
				this->add_entity(other_scene_entity);

			std::unordered_map<std::string, ComponentID> other_scene_entity_component_ids = other_scene_entity->get_component_ids();

			for (const auto& [component_name, id] : other_scene_entity_component_ids)
				this->add_component_to_last_entity(other_scene->get_component_by_id(id));
		}
	}
}

void Omnific::Scene::merge_another_scene_to_parent_entity_by_name(std::shared_ptr<Scene> other_scene, std::string parent_entity_name)
{
	std::shared_ptr<Entity> parent_entity = this->get_entity_by_name(parent_entity_name);
	if (parent_entity)
		this->merge_another_scene_to_parent_entity(other_scene, parent_entity->id);
}

void Omnific::Scene::set_entity_name(EntityID entity_id, std::string name)
{
	if (this->entity_names.count(name))
		if (this->entity_names.at(name) != entity_id)
			name += " (Copy)";

	std::shared_ptr<Entity> entity = this->get_entity(entity_id);
	if (entity != nullptr)
	{
		this->entity_names.erase(entity->get_name());
		entity->name = name;
		this->entity_names.emplace(name, entity_id);

		if (this->is_active())
		{
			EventBus::publish_event(OMNIFIC_EVENT_ENTITY_NAME_SET);
		}
	}
}

void Omnific::Scene::add_entity_tag(EntityID entity_id, std::string tag)
{
	std::shared_ptr<Entity> entity = this->get_entity(entity_id);
	if (entity != nullptr)
	{
		entity->tags.push_back(tag);
		this->entity_tags.emplace(tag, entity_id);

		if (this->is_active())
		{
			EventBus::publish_event(OMNIFIC_EVENT_ENTITY_TAG_SET);
		}
	}
}

void Omnific::Scene::add_component(EntityID entity_id, std::shared_ptr<Component> component)
{
	if (component != nullptr)
	{
		std::string type = component->get_type();
		std::shared_ptr<Entity> entity = this->entities.at(entity_id);

		if (entity != nullptr)
		{			
			//this->remove_component(entity_id, type);
			component->entity_id = entity_id;
			component->entity_name = entity->get_name();
			this->components.push_back(component);
			this->components_by_id.emplace(component->get_id(), component);
			entity->component_ids.emplace(type, component->get_id());
			size_t last_index = this->components.size() - 1;

			if (this->component_index_caches.count(type) > 0)
			{
				this->component_index_caches.at(type).push_back(last_index);
			}
			else
			{
				std::vector<size_t> component_indices;
				component_indices.push_back(last_index);
				this->component_index_caches.emplace(type, component_indices);
			}

			if (this->is_active())
			{
				EventBus::publish_event(OMNIFIC_EVENT_COMPONENT_ADDED, {}, {}, {}, {{"component", component}, {component->get_type(), component}});
				EventBus::publish_event(OMNIFIC_EVENT_ENTITY_MODIFIED);
				EventBus::publish_event(OMNIFIC_EVENT_SCENE_MODIFIED);
			}

			if (component->is_renderable())
			{
				entity->model_id = component->get_id();
				this->render_order_index_cache.push_back(last_index);
			}

			std::shared_ptr<Entity> viewport_entity = this->get_entity_by_name("Viewport");
			if (viewport_entity != nullptr)
			{
				std::shared_ptr<Viewport> viewport = this->get_component_by_type<Viewport>(viewport_entity->get_id());
				if (viewport != nullptr)
				{
					if (viewport->get_camera_entity_name() == "")
					{
						if (std::dynamic_pointer_cast<Camera>(component) != nullptr)
						{
							viewport->set_camera_entity_name(entity->get_name());
						}
					}
				}
			}
		}
	}
}

void Omnific::Scene::add_component_to_last_entity(std::shared_ptr<Component> component)
{
	this->add_component(this->last_entity_id, component);
}

void Omnific::Scene::remove_entity(EntityID entity_id)
{
	if (this->entities.count(entity_id) > 0)
	{
		std::shared_ptr<Entity> entity = this->get_entity(entity_id);
		std::unordered_map<std::string, ComponentID> entity_component_ids = entity->component_ids;

		/* Remove the children */
		std::vector<EntityID> child_ids = this->get_entity(entity_id)->child_ids;
		
		for (int i = 0; i < child_ids.size(); ++i)
			this->remove_entity(child_ids.at(i));

		for (auto& [component_name, component_id] : entity_component_ids)
			this->remove_component(entity_id, component_name);

		/* Remove the ID from the parent children list */
		std::shared_ptr<Entity> parent_entity = this->get_entity(this->get_entity(entity_id)->parent_id);

		for (auto it = parent_entity->child_ids.begin(); it != parent_entity->child_ids.end();)
		{
			if ((*it) == entity_id)
			{
				it = parent_entity->child_ids.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}

		/* Remove the entity itself*/
		this->entity_names.erase(entity->get_name());
		this->entities.erase(entity_id);

		if (this->is_active())
		{
			EventBus::publish_event(OMNIFIC_EVENT_ENTITY_REMOVED);
			EventBus::publish_event(OMNIFIC_EVENT_SCENE_MODIFIED);
		}
	}
}

void Omnific::Scene::remove_component(EntityID entity_id, std::string type)
{
	if (this->entities.count(entity_id) > 0)
	{
		std::shared_ptr<Entity> entity = this->get_entity(entity_id);

		if (entity->component_ids.count(type) > 0)
		{
			ComponentID component_id = entity->component_ids.at(type);
			entity->component_ids.erase(type);

			/* Remove the component from the list. */

			for (size_t i = 0; i < this->components.size();)
			{
				std::shared_ptr<Component> component = this->components.at(i);

				if (component != nullptr)
				{
					if (component->get_id() == component_id)
					{
						if (this->is_active())
						{
							EventBus::publish_event(OMNIFIC_EVENT_COMPONENT_REMOVED, {}, {}, {}, {{"component", component}, {component->get_type(), component}});
						}

						/*Blanks out the component instead of erasing so index caches 
						do not have to be rebuilt. */
						std::shared_ptr<Component> blank_component;
						this->components_by_id.erase(component_id);
						this->components.at(i) = blank_component;

						/*Remove from its respective component index cache*/
						std::vector<size_t> component_index_caches_by_type = component_index_caches.at(type);
						for (size_t j = 0; j < component_index_caches_by_type.size();)
						{
							if (component_index_caches_by_type.at(j) == i)
							{
								component_index_caches_by_type.erase(component_index_caches_by_type.begin() + j);
								break;
							}
							else
							{
								++j;
							}
						}

						/*Remove from the render order index cache*/
						for (size_t j = 0; j < this->render_order_index_cache.size();)
						{
							if (this->render_order_index_cache.at(j) == i)
							{
								this->render_order_index_cache.erase(this->render_order_index_cache.begin() + j);
								break;
							}
							else
							{
								++j;
							}
						}
						break;
					}
					else
					{
						++i;
					}
				}
				else
				{
					++i;
				}
			}

			if (this->is_active())
			{
				EventBus::publish_event(OMNIFIC_EVENT_ENTITY_MODIFIED);
				EventBus::publish_event(OMNIFIC_EVENT_SCENE_MODIFIED);
			}
		}
	}
}

std::shared_ptr<Omnific::Transform> Omnific::Scene::calculate_global_transform(EntityID local_transform_entity_id)
{
	std::shared_ptr<Transform> global_transform;
	std::shared_ptr<Entity> current_entity = this->get_entity(local_transform_entity_id);

	if (current_entity != nullptr)
	{
		std::vector<std::shared_ptr<Transform>> local_transforms;

		/* Find the root transform. */

		local_transforms.push_back(current_entity->transform);

		while (current_entity->parent_id != 0)
		{
			current_entity = this->get_entity(current_entity->parent_id);
			local_transforms.push_back(current_entity->transform);
		}

		glm::mat4 global_matrix = glm::mat4(1.0f);

		for (int i = local_transforms.size() - 1; i >= 0; --i)
		{
			std::shared_ptr<Transform> local_transform = local_transforms[i];
			glm::mat4 local_matrix = local_transform->get_transform_matrix();
			global_matrix *= local_matrix;
		}

		glm::vec3 skew;
		glm::vec4 perspective;
		glm::quat orientation_quat;
		glm::vec3 translation, scale;

		glm::decompose(global_matrix, scale, orientation_quat, translation, skew, perspective);

		glm::vec3 euler_angles = glm::degrees(glm::eulerAngles(orientation_quat));

		if (this->cached_global_transforms.count(local_transform_entity_id) == 0)
			this->cached_global_transforms.emplace(local_transform_entity_id, std::make_shared<Transform>());

		global_transform = this->cached_global_transforms.at(local_transform_entity_id);
		global_transform->translation = translation;
		global_transform->rotation = euler_angles;
		global_transform->scale = scale;
	}

    return global_transform;
}

std::vector<size_t> Omnific::Scene::get_render_order_index_cache()
{
	return this->render_order_index_cache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnific::Scene::get_component_index_caches()
{
	return this->component_index_caches;
}

void Omnific::Scene::clear_start_entity_queue()
{
	while (!this->start_entities_queue.empty())
		this->start_entities_queue.pop();
}

void Omnific::Scene::clear_finish_entity_queue()
{
	while (!this->finish_entities_queue.empty())
		this->finish_entities_queue.pop();
}

bool Omnific::Scene::is_active()
{
	for (auto& change_scene_request_event : EventBus::query_events(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED))
	{
		if (change_scene_request_event.get_parameters().strings.at("scene_name") == this->name)
		{
			return true;
		}
	}

	return SceneStorage::get_active_scene_name() == this->name || 
		SceneStorage::get_active_scene_name() == "";
}

std::queue<Omnific::EntityID> Omnific::Scene::get_start_entity_queue()
{
	return this->start_entities_queue;
}

std::queue<Omnific::EntityID> Omnific::Scene::get_finish_entity_queue()
{
	return this->finish_entities_queue;
}

std::vector<std::shared_ptr<Omnific::Component>> Omnific::Scene::get_components()
{
	return this->components;
}

std::shared_ptr<Omnific::Entity> Omnific::Scene::get_entity(EntityID entity_id)
{
	std::shared_ptr<Entity> entity;

	if (this->entities.count(entity_id))
		entity = this->entities.at(entity_id);

	return entity;
}

std::shared_ptr<Omnific::Entity> Omnific::Scene::get_entity_by_name(std::string name)
{
	std::shared_ptr<Entity> entity;

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second->name == name)
			entity = it->second;

	return entity;
}

std::shared_ptr<Omnific::Entity> Omnific::Scene::get_last_entity()
{
	return this->entities.at(this->last_entity_id);
}

std::unordered_map<Omnific::EntityID, std::shared_ptr<Omnific::Entity>>& Omnific::Scene::get_entities()
{
	return this->entities;
}

std::shared_ptr<Omnific::Component> Omnific::Scene::get_component_by_id(ComponentID component_id)
{
	std::shared_ptr<Component> component;

	if (this->components_by_id.count(component_id) > 0)
	{
		component = this->components_by_id.at(component_id);
	}

	return component;
}

std::shared_ptr<Omnific::Component> Omnific::Scene::get_component(std::string type, EntityID entity_id)
{
	std::shared_ptr<Entity> entity = this->get_entity(entity_id);
	std::shared_ptr<Component> component;

	if (entity->component_ids.count(type) > 0)
		component = this->get_component_by_id(entity->component_ids.at(type));

	return component;
}

void Omnific::Scene::load_from_gltf(std::string filepath)
{
	tinygltf::Model gltf_model;
	tinygltf::TinyGLTF tiny_gltf;
	std::string err;
	std::string warn;
	bool ret;
	std::string delimitter = "Scene::";
	size_t delimitter_position = filepath.find(delimitter);
	size_t position = delimitter_position + delimitter.length();

	if (delimitter_position < filepath.length())
	{
		std::string token = filepath.substr(position, filepath.length());
		unsigned char* data = 0;
		unsigned int size = 0;

		if (token == "cone")
		{
			data = DefaultAssets::cone_glb;
			size = DefaultAssets::cone_glb_len;
		}
		else if (token == "cube")
		{
			data = DefaultAssets::cube_glb;
			size = DefaultAssets::cube_glb_len;
		}
		else if (token == "cylinder")
		{
			data = DefaultAssets::cylinder_glb;
			size = DefaultAssets::cylinder_glb_len;
		}
		else if (token == "icosphere")
		{
			data = DefaultAssets::icosphere_glb;
			size = DefaultAssets::icosphere_glb_len;
		}	
		else if (token == "monkey")
		{
			data = DefaultAssets::monkey_glb;
			size = DefaultAssets::monkey_glb_len;
		}	
		else if (token == "plane")
		{
			data = DefaultAssets::plane_glb;
			size = DefaultAssets::plane_glb_len;
		}		
		else if (token == "sphere")
		{
			data = DefaultAssets::sphere_glb;
			size = DefaultAssets::sphere_glb_len;
		}	
		else if (token == "torus")
		{
			data = DefaultAssets::torus_glb;
			size = DefaultAssets::torus_glb_len;
		}
			
		ret = tiny_gltf.LoadBinaryFromMemory(
		&gltf_model,
		&err, 
		&warn, 
		data,
		size);
	}
	else
	{
		ret = tiny_gltf.LoadBinaryFromFile(
			&gltf_model,
			&err, 
			&warn, 
			Platform::get_file_access().find_path(filepath));
	}

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
		this->add_entity(gltf_scene_root_entity);

		for (size_t i = 0; i < gltf_model.nodes.size(); ++i)
		{
			tinygltf::Node gltf_node = gltf_model.nodes[i];
			int mesh_index = gltf_node.mesh;

			if (mesh_index != -1)
			{
				// GLTF data
				std::vector<float> positions = this->read_gltf_primitive_attribute(gltf_model, "POSITION", mesh_index);
				std::vector<float> texture_coords = this->read_gltf_primitive_attribute(gltf_model, "TEXCOORD_0", mesh_index);
				std::vector<float> normals = this->read_gltf_primitive_attribute(gltf_model, "NORMAL", mesh_index);
				std::vector<uint32_t> indices = this->read_gltf_primitive_indices(gltf_model, mesh_index);

				std::shared_ptr<Mesh> mesh(new Mesh(positions, texture_coords, normals, indices));
				std::shared_ptr<Material> material(new Material());

				material->albedo_map = std::shared_ptr<Image>(new Image("Image::#FFFFFFFF"));
				material->metallic_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
				material->specular_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
				material->roughness_map = std::shared_ptr<Image>(new Image("Image::#FFFFFFFF"));
				material->anisotropic_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
				material->normal_map = std::shared_ptr<Image>(new Image("Image::#8080FFFF"));
				material->emission_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
				material->displacement_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
				material->occlusion_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));

				std::shared_ptr<Entity> entity(new Entity());
				entity->parent_id = gltf_scene_root_entity->get_id();
				this->add_entity(entity);
				std::shared_ptr<Transform> transform = entity->get_transform();
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
						material->albedo_map = this->read_gltf_image(gltf_model, base_colour_texture_index);
					}
					else
					{
						std::vector<double> base_color_factor = gltf_material.pbrMetallicRoughness.baseColorFactor;

						if (base_color_factor.size() == 3)
						{
							material->albedo_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
								base_color_factor[0],
								base_color_factor[1],
								base_color_factor[2],
								1.0))
							));
						}
						else if (base_color_factor.size() == 4)
						{
							material->albedo_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
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
						material->metallic_map = this->read_gltf_image(gltf_model, metallic_rougness_texture_index);
					}
					else
					{
						double metallic_factor = gltf_material.pbrMetallicRoughness.metallicFactor;
						double roughness_factor = gltf_material.pbrMetallicRoughness.roughnessFactor;

						material->metallic_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							metallic_factor,
							metallic_factor,
							metallic_factor,
							1.0))
						));

						material->roughness_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							roughness_factor,
							roughness_factor,
							roughness_factor,
							1.0))
						));

					}

					/* Normal Map*/
					if (normal_texture_index != -1)
					{
						material->normal_map = this->read_gltf_image(gltf_model, normal_texture_index);
					}
					else
					{
						material->normal_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							0.5,
							0.5,
							1.0,
							1.0))
						));
					}

					/* Emission */
					if (emissive_texture_index != -1)
					{
						material->emission_map = this->read_gltf_image(gltf_model, emissive_texture_index);
					}
					else
					{
						std::vector<double> emissive_factor = gltf_material.emissiveFactor;

						material->emission_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(
							emissive_factor[0],
							emissive_factor[1],
							emissive_factor[2],
							1.0))
						));
					}

					/* Occlusion */
					if (occlusion_texture_index != -1)
					{
						material->occlusion_map = this->read_gltf_image(gltf_model, occlusion_texture_index);
					}

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
				model->set_shader(std::shared_ptr<Shader>(new Shader("Shader::PBR")));
				this->add_component_to_last_entity(std::dynamic_pointer_cast<Component>(model));
			}
		}
	}
}

std::vector<uint8_t> Omnific::Scene::read_gltf_buffer(std::vector<unsigned char> buffer_data, tinygltf::BufferView buffer_view)
{
	std::vector<uint8_t> bytes(
		buffer_data.begin() + buffer_view.byteOffset,
		buffer_data.begin() + buffer_view.byteOffset + buffer_view.byteLength);

	return bytes;
}

std::vector<float> Omnific::Scene::read_gltf_primitive_attribute(tinygltf::Model model, std::string attribute_name, size_t index)
{
	std::vector<float> attribute;
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);

	if (primitive.attributes.count(attribute_name) > 0)
	{
		tinygltf::Accessor accessor = model.accessors.at(primitive.attributes.at(attribute_name));
		tinygltf::BufferView buffer_view = model.bufferViews.at(accessor.bufferView);
		std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
		std::vector<uint8_t> bytes = this->read_gltf_buffer(buffer, buffer_view);
		float* float_byte_data = (float*)bytes.data();
		size_t float_byte_size = bytes.size() / sizeof(float);

		for (size_t i = 0; i < float_byte_size; ++i)
			attribute.push_back(float_byte_data[i]);
	}

	return attribute;
}

std::vector<uint32_t> Omnific::Scene::read_gltf_primitive_indices(tinygltf::Model model, size_t index)
{
	tinygltf::Primitive primitive = model.meshes.at(index).primitives.at(0);
	tinygltf::Accessor accessor = model.accessors.at(primitive.indices);
	tinygltf::BufferView buffer_view = model.bufferViews.at(accessor.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
	std::vector<uint8_t> index_bytes = this->read_gltf_buffer(buffer, buffer_view);
	std::vector<uint32_t> indices;

	uint16_t* short_index_byte_data = (uint16_t*)index_bytes.data();
	size_t short_index_byte_size = index_bytes.size() / sizeof(uint16_t);

	for (size_t i = 0; i < short_index_byte_size; ++i)
		indices.push_back((uint32_t)short_index_byte_data[i]);

	return indices;
}

std::shared_ptr<Omnific::Image> Omnific::Scene::read_gltf_image(tinygltf::Model model, int texture_index)
{
	int image_index = model.textures[texture_index].source;
	tinygltf::Image gltf_image = model.images[image_index];
	tinygltf::BufferView buffer_view = model.bufferViews.at(gltf_image.bufferView);
	std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;
	std::vector<uint8_t> image_file_bytes = this->read_gltf_buffer(buffer, buffer_view);
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

std::string Omnific::Scene::get_name()
{
	return this->name;
}

Omnific::SceneID Omnific::Scene::get_id()
{
	return this->id;
}

void Omnific::Scene::update_debug_statistics()
{
	std::string debug_string = Profiler::get_clock_deltas_to_string_by_tag("total");
	std::shared_ptr<GUI> debug_gui = this->get_component_by_type<GUI>(this->get_entity_by_name("debug_gui_entity")->get_id());
	Inputs& inputs = Platform::get_inputs();
	const int monitor_time_period = 1;

	this->fps_monitor_clock->set_end();

	if (fps_monitor_clock->get_delta_in_seconds() >= monitor_time_period)
	{
		fps_monitor_clock->set_start();
		if (debug_gui->get_alpha_in_percentage() > 0.1)
		{
			debug_string += Profiler::get_rendering_backend_name();
			debug_gui->set_to_label(debug_string);
		}
	}

	if (inputs.is_on_release("f3"))
	{
		if (debug_gui->get_alpha_in_percentage() < 0.5)
		{
			debug_gui->show();
		}
		else
		{
			debug_gui->hide();
		}
	}
}