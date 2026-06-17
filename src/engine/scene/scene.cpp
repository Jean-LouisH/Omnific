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
#include <cmath>
#include <foundations/singletons/profiler.hpp>

#include <foundations/singletons/event_bus.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/viewport.hpp>
#include <scene/components/renderable.hpp>
#include <scene/components/skeleton.hpp>
#include <scene/components/animator.hpp>
#include <scene/components/label.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

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
				this->set_entity_name(viewport_entity->get_id(), DEFAULT_VIEWPORT_NAME);
				this->add_component_to_last_entity(viewport);

				for (YAML::const_iterator it1 = yaml_node.begin(); it1 != yaml_node.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity" || it1->first.as<std::string>() == "Scene")
					{
						std::shared_ptr<Entity> entity(new Entity());
						this->add_entity(entity);
						std::string name;
						std::string source;

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							//Entity attributes
							if (it2->first.as<std::string>() == "name")
							{
								name = it2->second.as<std::string>();
								this->set_entity_name(this->get_last_entity()->get_id(), name);
							}
							else if (it2->first.as<std::string>() == "dimension")
							{
								if (it2->second.as<int>() == 2)
									this->get_last_entity()->is_2d = true;
							}
							else if (it2->first.as<std::string>() == "source")
							{
								source = it2->second.as<std::string>();
								if (name == "")
									this->set_entity_name(this->get_last_entity()->get_id(), source);
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
										transform->set_x_rotation(it3->second[0].as<double>());
										transform->set_y_rotation(it3->second[1].as<double>());
										transform->set_z_rotation(it3->second[2].as<double>());
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
							if (source != filepath)
							{
								this->merge_another_scene_to_parent_entity(std::shared_ptr<Scene>(new Scene(source)), entity->get_id());
							}
						}
					}
				}
			}
			catch (int e)
			{

			}

			std::shared_ptr<Entity> debug_label_entity = std::make_shared<Entity>("debug_label_entity");
			std::shared_ptr<Label> debug_label = std::make_shared<Label>();

			debug_label_entity->is_2d = true;
			debug_label->hide();
			debug_label->pivot = GUIElement::PivotPoint::TOP_RIGHT;
			debug_label->anchor_pivot = GUIElement::PivotPoint::TOP_RIGHT;
			debug_label->margin = glm::vec2(-10.0f,-10.0f);
			debug_label->set_text("");
			debug_label->set_horizontal_alignment(Font::HorizontalAlignment::RIGHT);
			this->add_entity(debug_label_entity);
			this->add_component_to_last_entity(debug_label);
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

void Omnific::Scene::add_empty_entity(std::string entity_name)
{
	std::shared_ptr<Entity> empty_entity(new Entity(entity_name));
	this->add_entity(empty_entity);
}

std::shared_ptr<Omnific::Entity> Omnific::Scene::add_preset_entity(EntityPreset preset)
{
	std::shared_ptr<Entity> preset_entity;

	switch (preset)
	{
		case EntityPreset::CUBE: ; break;
		case EntityPreset::CUBE_WITH_PHYSICS: ; break;
		case EntityPreset::CHARACTER: ; break;
		case EntityPreset::CAMERA: ; break;
		case EntityPreset::LIGHT: ; break;
	}

	return preset_entity;
}

std::shared_ptr<Omnific::Entity> Omnific::Scene::add_preset_entity_by_string(std::string preset_string)
{
	std::shared_ptr<Entity> preset_entity;

	if (preset_string == "EntityPreset::CUBE")
	{
		return this->add_preset_entity(EntityPreset::CUBE);
	}
	else if (preset_string == "EntityPreset::CUBE_WITH_PHYSICS")
	{
		return this->add_preset_entity(EntityPreset::CUBE_WITH_PHYSICS);
	}
	else if (preset_string == "EntityPreset::CHARACTER")
	{
		return this->add_preset_entity(EntityPreset::CHARACTER);
	}
	else if (preset_string == "EntityPreset::CAMERA")
	{
		return this->add_preset_entity(EntityPreset::CAMERA);
	}

	return preset_entity;
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
		std::shared_ptr<Entity> viewport_entity = other_scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME);
		std::shared_ptr<Entity> debug_camera_entity = other_scene->get_entity_by_name("debug_camera_entity");
		std::shared_ptr<Entity> debug_label_entity = other_scene->get_entity_by_name("debug_label_entity");
		
		if (viewport_entity != nullptr)
			other_scene->remove_entity(viewport_entity->get_id());
		if (debug_camera_entity != nullptr)
			other_scene->remove_entity(debug_camera_entity->get_id());
		if (debug_label_entity != nullptr)
			other_scene->remove_entity(debug_label_entity->get_id());

		/* Transfer Entities and their Components */
		std::unordered_map<EntityID, std::shared_ptr<Entity>> other_scene_entities = other_scene->get_entities();

		/*Entities without parents are listed before others.*/
		std::vector<std::shared_ptr<Entity>> sorted_entities;
		std::shared_ptr<Entity> root_entity;

		// Find the root entity, add it, then add its children, then their children, etc.
		for (auto& [entity_id, entity] : other_scene_entities)
		{
			if (entity->parent_id == 0)
				root_entity = entity;
		}

		if (root_entity)
		{
			sorted_entities.push_back(root_entity);

			for (auto& child_id : root_entity->child_ids)
			{
				std::shared_ptr<Entity> child_entity = other_scene->get_entity(child_id);
				sorted_entities.push_back(child_entity);

				std::vector<EntityID> entities_to_visit;
				entities_to_visit.insert(entities_to_visit.end(), child_entity->child_ids.begin(), child_entity->child_ids.end());

				while (!entities_to_visit.empty())
				{
					EntityID entity_id = entities_to_visit.at(0);
					entities_to_visit.erase(entities_to_visit.begin());

					std::shared_ptr<Entity> entity = other_scene->get_entity(entity_id);
					sorted_entities.push_back(entity);

					entities_to_visit.insert(entities_to_visit.end(), entity->child_ids.begin(), entity->child_ids.end());
				}
			}
		}


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
			name += " (ID: " + std::to_string(entity_id) + ")";

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
				entity->renderable_id = component->get_id();
				this->rendering_order_index_cache.push_back(last_index);
			}

			std::shared_ptr<Entity> viewport_entity = this->get_entity_by_name(DEFAULT_VIEWPORT_NAME);
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

						component->entity_id = 0;
						component->entity_name = "";

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
						for (size_t j = 0; j < this->rendering_order_index_cache.size();)
						{
							if (this->rendering_order_index_cache.at(j) == i)
							{
								this->rendering_order_index_cache.erase(this->rendering_order_index_cache.begin() + j);
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

		if (this->cached_global_transforms.count(local_transform_entity_id) == 0)
			this->cached_global_transforms.emplace(local_transform_entity_id, std::make_shared<Transform>());

		global_transform = this->cached_global_transforms.at(local_transform_entity_id);
		global_transform->translation = translation;
		global_transform->rotation = orientation_quat;
		global_transform->scale = scale;
	}

    return global_transform;
}

std::vector<size_t> Omnific::Scene::get_rendering_order_index_cache()
{
	return this->rendering_order_index_cache;
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
	for (auto& servicing_scene_change_event : EventBus::query_events(OMNIFIC_EVENT_SERVICING_SCENE_CHANGE))
	{
		if (servicing_scene_change_event.get_parameters().strings.at("scene_name") == this->name)
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

std::unordered_map<Omnific::EntityID, std::shared_ptr<Omnific::Entity>> Omnific::Scene::get_entities()
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

std::shared_ptr<Omnific::Component> Omnific::Scene::get_component_from_entity_by_name(std::string type, std::string entity_name)
{
	std::shared_ptr<Entity> entity = this->get_entity_by_name(entity_name);
	std::shared_ptr<Component> component;

	if (entity)
		component = this->get_component(type, entity->get_id());

	return component;
}

std::vector<std::shared_ptr<Omnific::Renderable>> Omnific::Scene::get_renderables_in_order()
{
	std::vector<std::shared_ptr<Renderable>> renderables;
	std::vector<std::shared_ptr<Component>> components = this->get_components();

	for (int i = 0; i < this->rendering_order_index_cache.size(); ++i)
	{
		std::shared_ptr<Renderable> renderable =
			std::dynamic_pointer_cast<Renderable>(components[this->rendering_order_index_cache[i]]);

		if (renderable != nullptr)
			renderables.push_back(renderable);
	}

	return renderables;
}

glm::vec3 Omnific::Scene::get_dimensions()
{
	glm::vec3 dimensions = glm::vec3(0.0f);

	for (const auto& [entity_id, entity] : this->entities)
	{
		std::shared_ptr<Renderable> renderable = this->get_component_by_type<Renderable>(entity_id);
		glm::vec3 renderable_dimensions = renderable != nullptr ? renderable->get_dimensions() : glm::vec3(0.0f);
		std::shared_ptr<Transform> global_transform = this->calculate_global_transform(entity_id);

		if (global_transform != nullptr)
		{
			 dimensions.x = std::max(dimensions.x, std::abs(global_transform->translation.x) + renderable_dimensions.x);
			 dimensions.y = std::max(dimensions.y, std::abs(global_transform->translation.y) + renderable_dimensions.y);
			 dimensions.z = std::max(dimensions.z, std::abs(global_transform->translation.z) + renderable_dimensions.z);
		}
	}

	return dimensions;
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

        if (token == "CONE") 
			{ data = DefaultAssets::cone_glb; size = DefaultAssets::cone_glb_len; }
        else if (token == "CUBE") 
			{ data = DefaultAssets::cube_glb; size = DefaultAssets::cube_glb_len; }
        else if (token == "CYLINDER") 
			{ data = DefaultAssets::cylinder_glb; size = DefaultAssets::cylinder_glb_len; }
        else if (token == "ICOSPHERE") 
			{ data = DefaultAssets::icosphere_glb; size = DefaultAssets::icosphere_glb_len; }    
        else if (token == "MONKEY") 
			{ data = DefaultAssets::monkey_glb; size = DefaultAssets::monkey_glb_len; }    
        else if (token == "PLANE") 
			{ data = DefaultAssets::plane_glb; size = DefaultAssets::plane_glb_len; }        
        else if (token == "SPHERE") 
			{ data = DefaultAssets::sphere_glb; size = DefaultAssets::sphere_glb_len; }    
        else if (token == "TORUS") 
			{ data = DefaultAssets::torus_glb; size = DefaultAssets::torus_glb_len; }
            
        ret = tiny_gltf.LoadBinaryFromMemory(&gltf_model, &err, &warn, data, size);
    }
    else
    {
        ret = tiny_gltf.LoadBinaryFromFile(&gltf_model, &err, &warn, Platform::get_file_access().find_path(filepath));
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
        std::shared_ptr<Entity> gltf_scene_root_entity(new Entity("GLTF root"));
        this->add_entity(gltf_scene_root_entity);

        std::map<int, uint32_t> gltf_node_to_entity_id;
        std::vector<std::pair<uint32_t, int>> entity_skin_assignments;

        int scene_index = gltf_model.defaultScene >= 0 ? gltf_model.defaultScene : 0;
        if (scene_index < gltf_model.scenes.size())
        {
            const tinygltf::Scene& scene = gltf_model.scenes[scene_index];
			this->set_entity_name(gltf_scene_root_entity->get_id(), scene.name); 
            for (int node_index : scene.nodes)
            {
                this->process_gltf_node(
					gltf_model, node_index, 
					gltf_scene_root_entity, 
					gltf_node_to_entity_id, 
					entity_skin_assignments
				);
            }
        }

        // Load and register skeletal rig structures if present
        if (!gltf_model.skins.empty())
        {
            this->load_gltf_skins(
				gltf_model, 
				gltf_node_to_entity_id, 
				entity_skin_assignments
			);
        }

        // Load and register time-series keyframe animation animations if present
        if (!gltf_model.animations.empty())
        {
            this->load_gltf_animations(
				gltf_model, 
				gltf_scene_root_entity, 
				gltf_node_to_entity_id
			);
        }
    }
}

void Omnific::Scene::process_gltf_node(const tinygltf::Model& model, int node_index, std::shared_ptr<Entity> parent_entity, 
                                      std::map<int, uint32_t>& node_map, std::vector<std::pair<uint32_t, int>>& skin_assignments)
{
    const tinygltf::Node& gltf_node = model.nodes[node_index];
    
    std::shared_ptr<Entity> node_entity(new Entity(gltf_node.name));
    node_entity->parent_id = parent_entity->get_id();
    this->add_entity(node_entity);
    
    node_map[node_index] = node_entity->get_id();
    
    if (gltf_node.skin != -1)
    {
        skin_assignments.push_back({ node_entity->get_id(), gltf_node.skin });
    }

    std::shared_ptr<Transform> transform = node_entity->get_transform();

    if (gltf_node.matrix.size() == 16)
    {
        glm::mat4 local_matrix = glm::make_mat4(gltf_node.matrix.data());
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(
			local_matrix, 
			transform->scale, 
			transform->rotation,
			transform->translation,
			skew, 
			perspective
		);
    }
    else
    {
        if (gltf_node.translation.size() == 3)
        {
            transform->translation = { gltf_node.translation[0], gltf_node.translation[1], gltf_node.translation[2] };
        }
        if (gltf_node.rotation.size() == 4)
        {
            transform->rotation = glm::quat(gltf_node.rotation[3], gltf_node.rotation[0], gltf_node.rotation[1], gltf_node.rotation[2]);
        }
        if (gltf_node.scale.size() == 3)
        {
            transform->scale = { gltf_node.scale[0], gltf_node.scale[1], gltf_node.scale[2] };
        }
    }

    int mesh_index = gltf_node.mesh;
    if (mesh_index != -1 && mesh_index < model.meshes.size())
    {
        const tinygltf::Mesh& gltf_mesh = model.meshes[mesh_index];

        for (const auto& primitive : gltf_mesh.primitives)
        {
            std::vector<float> positions = this->read_gltf_primitive_attribute(model, "POSITION", primitive);
            std::vector<float> texture_coords = this->read_gltf_primitive_attribute(model, "TEXCOORD_0", primitive);
            std::vector<float> normals = this->read_gltf_primitive_attribute(model, "NORMAL", primitive);
			std::vector<float> tangents = this->read_gltf_primitive_attribute(model, "TANGENT", primitive);
            std::vector<uint32_t> indices = this->read_gltf_primitive_indices(model, primitive);
            std::vector<uint32_t> joints = this->read_gltf_primitive_joint_attribute(model, primitive);
            std::vector<float> weights = this->read_gltf_primitive_attribute(model, "WEIGHTS_0", primitive);

            std::shared_ptr<Mesh> mesh(new Mesh(positions, texture_coords, normals, tangents, indices, joints, weights));
            std::shared_ptr<Renderable::Material> material(new Renderable::Material());

            material->albedo_map = std::shared_ptr<Image>(new Image("Image::#FFFFFFFF"));
            material->metallic_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
            material->specular_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
            material->roughness_map = std::shared_ptr<Image>(new Image("Image::#FFFFFFFF"));
            material->anisotropic_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
            material->normal_map = std::shared_ptr<Image>(new Image("Image::#8080FFFF"));
            material->emission_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
            material->displacement_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));
            material->occlusion_map = std::shared_ptr<Image>(new Image("Image::#000000FF"));

            int material_index = primitive.material;
            if (material_index != -1 && material_index < model.materials.size())
            {
                const tinygltf::Material& gltf_material = model.materials[material_index];
                int base_colour_texture_index = gltf_material.pbrMetallicRoughness.baseColorTexture.index;
                int metallic_rougness_texture_index = gltf_material.pbrMetallicRoughness.metallicRoughnessTexture.index;
                int normal_texture_index = gltf_material.normalTexture.index;
                int emissive_texture_index = gltf_material.emissiveTexture.index;
                int occlusion_texture_index = gltf_material.occlusionTexture.index;

                if (base_colour_texture_index != -1) { material->albedo_map = this->read_gltf_image(model, base_colour_texture_index); }
                else 
                {
                    const auto& base_color_factor = gltf_material.pbrMetallicRoughness.baseColorFactor;

                    if (base_color_factor.size() == 3) 
					{ material->albedo_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(base_color_factor[0], base_color_factor[1], base_color_factor[2], 1.0)))); }
                    else if (base_color_factor.size() == 4) 
					{ material->albedo_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(base_color_factor[0], base_color_factor[1], base_color_factor[2], base_color_factor[3])))); }
                }

                if (metallic_rougness_texture_index != -1) { material->metallic_map = this->read_gltf_image(model, metallic_rougness_texture_index); }
                else 
                {
                    double metallic_factor = gltf_material.pbrMetallicRoughness.metallicFactor;
                    double roughness_factor = gltf_material.pbrMetallicRoughness.roughnessFactor;
                    material->metallic_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(metallic_factor, metallic_factor, metallic_factor, 1.0))));
                    material->roughness_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(roughness_factor, roughness_factor, roughness_factor, 1.0))));
                }

                if (normal_texture_index != -1) 
					{ material->normal_map = this->read_gltf_image(model, normal_texture_index); }
                else 
					{ material->normal_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(0.5, 0.5, 1.0, 1.0)))); }

                if (emissive_texture_index != -1) { material->emission_map = this->read_gltf_image(model, emissive_texture_index); }
                else if (gltf_material.emissiveFactor.size() == 3) 
                {
                    const auto& emissive_factor = gltf_material.emissiveFactor;
                    material->emission_map = std::shared_ptr<Image>(new Image(std::shared_ptr<Colour>(new Colour(emissive_factor[0], emissive_factor[1], emissive_factor[2], 1.0))));
                }

                if (occlusion_texture_index != -1) { material->occlusion_map = this->read_gltf_image(model, occlusion_texture_index); }
            }

            std::shared_ptr<Entity> primitive_entity(new Entity(gltf_mesh.name));
            primitive_entity->parent_id = node_entity->get_id();
            this->add_entity(primitive_entity);

            std::shared_ptr<Renderable> renderable(new Renderable());
            renderable->material = material;
            renderable->mesh = mesh;
			renderable->set_dimensions(mesh->get_dimensions());
            renderable->set_shader(std::shared_ptr<Shader>(new Shader("Shader::PBR")));
            this->add_component_to_last_entity(std::dynamic_pointer_cast<Component>(renderable));
        }
    }

    for (int child_index : gltf_node.children)
    {
        this->process_gltf_node(
			model, 
			child_index,
			node_entity, 
			node_map, 
			skin_assignments
		);
    }
}

void Omnific::Scene::load_gltf_skins(const tinygltf::Model& model, const std::map<int, uint32_t>& node_map, const std::vector<std::pair<uint32_t, int>>& skin_assignments)
{
    std::map<int, std::shared_ptr<Skeleton>> loaded_skeletons;

    for (size_t i = 0; i < model.skins.size(); ++i)
    {
        const auto& gltf_skin = model.skins[i];
        std::shared_ptr<Skeleton> skeleton(new Skeleton());
        skeleton->skin_name = gltf_skin.name;

        for (int joint_node_idx : gltf_skin.joints)
        {
            if (node_map.count(joint_node_idx) > 0)
            {
                skeleton->joint_entity_ids.push_back(node_map.at(joint_node_idx));
            }
        }

        if (gltf_skin.inverseBindMatrices != -1)
        {
            const auto& accessor = model.accessors.at(gltf_skin.inverseBindMatrices);
            const auto& buffer_view = model.bufferViews.at(accessor.bufferView);
            const auto& buffer = model.buffers.at(buffer_view.buffer);

            size_t stride = accessor.ByteStride(buffer_view);
            const unsigned char* data_ptr = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;

            for (size_t k = 0; k < accessor.count; ++k)
            {
                glm::mat4 ibm = glm::make_mat4(reinterpret_cast<const float*>(data_ptr + (k * stride)));
                skeleton->inverse_bind_matrices.push_back(ibm);
            }
        }
        loaded_skeletons[static_cast<int>(i)] = skeleton;
    }

    for (const auto& [target_entity_id, skin_index] : skin_assignments)
    {
        if (loaded_skeletons.count(skin_index) > 0)
        {
			this->add_component(target_entity_id, loaded_skeletons[skin_index]);
        }
    }
}

void Omnific::Scene::load_gltf_animations(const tinygltf::Model& model, std::shared_ptr<Entity> root_entity, const std::map<int, uint32_t>& node_map)
{
	std::shared_ptr<Animator> animator = std::make_shared<Animator>();

    for (const auto& gltf_anim : model.animations)
    {
        std::shared_ptr<SkeletalAnimation> skeletal_animation(new SkeletalAnimation());
        skeletal_animation->name = gltf_anim.name;

        for (const auto& gltf_sampler : gltf_anim.samplers)
        {
            SkeletalAnimation::Sampler sampler;
            
            if (gltf_sampler.interpolation == "LINEAR") 
				sampler.interpolation = Animation::InterpolationType::LINEAR;
            else if (gltf_sampler.interpolation == "STEP") 
				sampler.interpolation = Animation::InterpolationType::STEP;
            else if (gltf_sampler.interpolation == "CUBICSPLINE") 
				sampler.interpolation = Animation::InterpolationType::CUBIC_SPLINE;

            // Extract Input Timestamps (Seconds)
            const auto& in_accessor = model.accessors.at(gltf_sampler.input);
            const auto& in_view = model.bufferViews.at(in_accessor.bufferView);
            const auto& in_buffer = model.buffers.at(in_view.buffer);
            const unsigned char* in_ptr = in_buffer.data.data() + in_view.byteOffset + in_accessor.byteOffset;
            size_t in_stride = in_accessor.ByteStride(in_view);

            for (size_t idx = 0; idx < in_accessor.count; ++idx)
            {
                sampler.input_timestamps.push_back(*reinterpret_cast<const float*>(in_ptr + (idx * in_stride)));
            }

            if (!sampler.input_timestamps.empty())
            {
                skeletal_animation->duration = std::max(skeletal_animation->duration, sampler.input_timestamps.back());
            }

            // Extract Transformation States Output Values
            const auto& out_accessor = model.accessors.at(gltf_sampler.output);
            const auto& out_view = model.bufferViews.at(out_accessor.bufferView);
            const auto& out_buffer = model.buffers.at(out_view.buffer);
            const unsigned char* out_ptr = out_buffer.data.data() + out_view.byteOffset + out_accessor.byteOffset;
            size_t out_stride = out_accessor.ByteStride(out_view);

            for (size_t idx = 0; idx < out_accessor.count; ++idx)
            {
                const float* val = reinterpret_cast<const float*>(out_ptr + (idx * out_stride));
                if (out_accessor.type == TINYGLTF_TYPE_VEC3)
                {
                    sampler.output_values.push_back(glm::vec4(val[0], val[1], val[2], 0.0f));
                }
                else if (out_accessor.type == TINYGLTF_TYPE_VEC4) // Quaternions
                {
                    sampler.output_values.push_back(glm::vec4(val[0], val[1], val[2], val[3]));
                }
            }
            skeletal_animation->samplers.push_back(sampler);
        }

        for (const auto& gltf_channel : gltf_anim.channels)
        {
            if (node_map.count(gltf_channel.target_node) == 0) 
				continue; // Skip targets that are missing

            SkeletalAnimation::Channel channel;
            channel.sampler_index = gltf_channel.sampler;
            channel.target_entity_id = node_map.at(gltf_channel.target_node);

            if (gltf_channel.target_path == "translation") 
				channel.path = SkeletalAnimation::Path::TRANSLATION;
            else if (gltf_channel.target_path == "rotation") 
				channel.path = SkeletalAnimation::Path::ROTATION;
            else if (gltf_channel.target_path == "scale") 
				channel.path = SkeletalAnimation::Path::SCALE;

            skeletal_animation->channels.push_back(channel);
        }
        
		animator->skeletal_animations.emplace(skeletal_animation->name, skeletal_animation);
    }

	this->add_component(root_entity->get_id(), animator);
}

std::vector<float> Omnific::Scene::read_gltf_primitive_attribute(const tinygltf::Model& model, const std::string& attribute_name, const tinygltf::Primitive& primitive)
{
    std::vector<float> attribute;
    if (primitive.attributes.count(attribute_name) == 0) return attribute;

    const tinygltf::Accessor& accessor = model.accessors.at(primitive.attributes.at(attribute_name));
    const tinygltf::BufferView& buffer_view = model.bufferViews.at(accessor.bufferView);
    const tinygltf::Buffer& buffer = model.buffers.at(buffer_view.buffer);

    size_t stride = accessor.ByteStride(buffer_view);
    const unsigned char* data_ptr = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;

    size_t num_components = 1;
    if (accessor.type == TINYGLTF_TYPE_VEC2) num_components = 2;
    else if (accessor.type == TINYGLTF_TYPE_VEC3) num_components = 3;
    else if (accessor.type == TINYGLTF_TYPE_VEC4) num_components = 4;

    attribute.reserve(accessor.count * num_components);

    for (size_t i = 0; i < accessor.count; ++i)
    {
        const float* ptr = reinterpret_cast<const float*>(data_ptr + (i * stride));
        for (size_t c = 0; c < num_components; ++c)
        {
            attribute.push_back(ptr[c]);
        }
    }
    return attribute;
}

std::vector<uint32_t> Omnific::Scene::read_gltf_primitive_joint_attribute(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
{
    std::vector<uint32_t> joints;

    if (primitive.attributes.count("JOINTS_0") == 0) 
		return joints;

    const tinygltf::Accessor& accessor = model.accessors.at(primitive.attributes.at("JOINTS_0"));
    const tinygltf::BufferView& buffer_view = model.bufferViews.at(accessor.bufferView);
    const tinygltf::Buffer& buffer = model.buffers.at(buffer_view.buffer);

    size_t stride = accessor.ByteStride(buffer_view);
    const unsigned char* data_ptr = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;

    size_t num_components = 4;
    joints.reserve(accessor.count * num_components);

    for (size_t i = 0; i < accessor.count; ++i)
    {
        const unsigned char* element_ptr = data_ptr + (i * stride);
        for (size_t c = 0; c < num_components; ++c)
        {
            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
            {
                joints.push_back(reinterpret_cast<const uint16_t*>(element_ptr)[c]);
            }
            else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
            {
                joints.push_back(reinterpret_cast<const uint8_t*>(element_ptr)[c]);
            }
        }
    }
    return joints;
}

std::vector<uint32_t> Omnific::Scene::read_gltf_primitive_indices(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
{
    std::vector<uint32_t> indices;

    if (primitive.indices == -1) 
		return indices;

    const tinygltf::Accessor& accessor = model.accessors.at(primitive.indices);
    const tinygltf::BufferView& buffer_view = model.bufferViews.at(accessor.bufferView);
    const tinygltf::Buffer& buffer = model.buffers.at(buffer_view.buffer);

    size_t stride = accessor.ByteStride(buffer_view);
    const unsigned char* data_ptr = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;

    indices.reserve(accessor.count);

    for (size_t i = 0; i < accessor.count; ++i)
    {
        const unsigned char* element_ptr = data_ptr + (i * stride);
        
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            indices.push_back(*reinterpret_cast<const uint32_t*>(element_ptr));
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            indices.push_back(*reinterpret_cast<const uint16_t*>(element_ptr));
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
        {
            indices.push_back(*reinterpret_cast<const uint8_t*>(element_ptr));
        }
    }

    return indices;
}

std::shared_ptr<Omnific::Image> Omnific::Scene::read_gltf_image(const tinygltf::Model& model, int texture_index)
{
    if (texture_index < 0 || texture_index >= model.textures.size()) 
		return nullptr;
    
    int image_index = model.textures[texture_index].source;
	
    if (image_index < 0 || image_index >= model.images.size()) 
		return nullptr;

    const tinygltf::Image& gltf_image = model.images[image_index];

	if (!gltf_image.image.empty())
	{
		tinygltf::BufferView buffer_view = model.bufferViews.at(gltf_image.bufferView);
		std::vector<unsigned char> buffer = model.buffers.at(buffer_view.buffer).data;

		std::vector<uint8_t> image_file_bytes(
			buffer.begin() + buffer_view.byteOffset,
			buffer.begin() + buffer_view.byteOffset + buffer_view.byteLength
		);

		int width = 0;
		int height = 0;
		int colour_channels = 0;

		stbi_set_flip_vertically_on_load(0);

		std::shared_ptr<uint8_t> image_data = std::shared_ptr<uint8_t>(stbi_load_from_memory(
			image_file_bytes.data(),
			image_file_bytes.size(),
			&width,
			&height,
			&colour_channels,
			0), stbi_image_free);

		return std::shared_ptr<Image>(new Image(
			image_data.get(),
			width,
			height,
			colour_channels));
	}
    
    return nullptr;
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
	std::shared_ptr<Label> debug_label = this->get_component_by_type<Label>(this->get_entity_by_name("debug_label_entity")->get_id());
	Inputs& inputs = Platform::get_inputs();
	const int monitor_time_period = 1;

	this->fps_monitor_clock->set_end();

	if (fps_monitor_clock->get_delta_in_seconds() >= monitor_time_period)
	{
		fps_monitor_clock->set_start();
		if (debug_label->get_alpha_in_percentage() > 0.1)
		{
			debug_string += Profiler::get_render_device_name();
			debug_label->set_text(debug_string);
		}
	}

	if (inputs.is_on_release("f3"))
	{
		if (debug_label->get_alpha_in_percentage() < 0.5)
		{
			debug_label->show();
		}
		else
		{
			debug_label->hide();
		}
	}
}