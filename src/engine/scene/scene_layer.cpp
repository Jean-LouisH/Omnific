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

#include "scene_layer.hpp"
#include "scene/components/component.hpp"
#include <foundations/singletons/event_bus.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/viewport.hpp>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtx/rotate_vector.hpp>

Omnific::SceneLayer::SceneLayer()
{
	this->id = UIDGenerator::get_new_uid();
	this->name = "SceneLayer (ID:" + std::to_string(this->id) + ")";

	// std::shared_ptr<Entity> camera_entity = std::shared_ptr<Entity>(new Entity());
	// std::shared_ptr<Transform> camera_transform = std::shared_ptr<Transform>(new Transform());
	// std::shared_ptr<Camera> camera = std::shared_ptr<Camera>(new Camera());
	// camera_entity->set_name("Camera");
	// this->add_entity(camera_entity);
	// this->add_component_to_last_entity(camera_transform);
	// this->add_component_to_last_entity(camera);
	// std::shared_ptr<Entity> viewport_entity = std::shared_ptr<Entity>(new Entity());
	// std::shared_ptr<Viewport> viewport = std::shared_ptr<Viewport>(new Viewport());
	// this->add_entity(viewport_entity);
	// viewport->set_camera_entity_name("Camera");
	// this->add_component_to_last_entity(viewport);
}

void Omnific::SceneLayer::add_entity(std::shared_ptr<Entity> entity)
{
	if (entity->parent_id != 0)
		this->entities.at(entity->parent_id)->child_ids.push_back(entity->id);

	this->start_entities_queue.emplace(entity->id);
	this->entities.emplace(entity->id, entity);
	this->last_entity_id = entity->id;
	this->set_entity_name(entity->id, entity->name);
	EventBus::publish(OMNIFIC_EVENT_ENTITY_ADDED);
}

void Omnific::SceneLayer::add_empty_entity()
{
	std::shared_ptr<Entity> empty_entity(new Entity());
	this->add_entity(empty_entity);
}

void Omnific::SceneLayer::set_entity_name(EntityID entity_id, std::string name)
{
	if (this->entity_names.count(name))
		name += "(Copy)";

	this->get_entity(entity_id)->name = name;
	this->entity_names.emplace(name, entity_id);
	EventBus::publish(OMNIFIC_EVENT_ENTITY_NAME_SET);
}

void Omnific::SceneLayer::add_entity_tag(EntityID entity_id, std::string tag)
{
	this->get_entity(entity_id)->tags.push_back(tag);
	this->entity_tags.emplace(tag, entity_id);
	EventBus::publish(OMNIFIC_EVENT_ENTITY_TAG_SET);
}

void Omnific::SceneLayer::add_component(EntityID entity_id, std::shared_ptr<Component> component)
{
	std::string type = component->get_type();
	std::shared_ptr<Entity> entity = this->entities.at(entity_id);

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

	EventBus::publish(OMNIFIC_EVENT_COMPONENT_ADDED);

	if (component->is_renderable())
	{
		entity->renderable_component_id = component->get_id();
		this->render_order_index_cache.push_back(last_index);
	}
}

void Omnific::SceneLayer::add_component_to_last_entity(std::shared_ptr<Component> component)
{
	this->add_component(this->last_entity_id, component);
}

void Omnific::SceneLayer::remove_entity(EntityID entity_id)
{
	if (this->entities.count(entity_id) > 0)
	{
		std::unordered_map<std::string, ComponentID> entity_component_ids = this->get_entity(entity_id)->component_ids;

		for (auto it = entity_component_ids.begin(); it != entity_component_ids.end(); it++)
			this->remove_component(entity_id, it->first);

		/* Remove the children */
		std::vector<EntityID> child_ids = this->get_entity(entity_id)->child_ids;

		for (int i = 0; i < child_ids.size(); i++)
			this->remove_entity(child_ids.at(i));

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

		this->entities.erase(entity_id);
		EventBus::publish(OMNIFIC_EVENT_ENTITY_REMOVED);
	}
}

void Omnific::SceneLayer::remove_component(EntityID entity_id, std::string type)
{
	if (this->entities.count(entity_id) > 0)
	{
		std::shared_ptr<Entity> entity = this->get_entity(entity_id);

		if (entity->component_ids.count(type) > 0)
		{
			ComponentID component_id = entity->component_ids.at(type);
			entity->component_ids.erase(type);

			/* Remove the component from the list. */

			for (auto it = this->components.begin(); it != this->components.end();)
			{
				if ((*it)->get_id() == component_id)
				{
					this->components_by_id.erase(component_id);
					it = this->components.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}

			EventBus::publish(OMNIFIC_EVENT_COMPONENT_REMOVED);

			/* Rebuild index caches */

			this->component_index_caches.clear();
			this->render_order_index_cache.clear();

			std::vector<size_t> component_indices;
			this->component_index_caches.emplace(type, component_indices);

			for (size_t i = 0; i < components.size(); i++)
			{
				this->component_index_caches.at(type).push_back(i);
				if (components.at(i)->is_renderable())
					this->render_order_index_cache.push_back(i);
			}
		}
	}
}

std::shared_ptr<Omnific::Transform> Omnific::SceneLayer::compute_global_transform(EntityID local_transform_entity_id)
{
	std::shared_ptr<Transform> global_transform = std::shared_ptr<Transform>(new Transform());
	std::shared_ptr<Entity> current_entity = this->get_entity(local_transform_entity_id);
	std::vector<std::shared_ptr<Transform>> local_transforms;

	/* Find the root transform. */

	local_transforms.push_back(current_entity->transform);

	while (current_entity->parent_id != 0)
	{
		current_entity = this->get_entity(current_entity->parent_id);
		local_transforms.push_back(current_entity->transform);
	}

	/* Accumulate the local transforms from the root transform and down. */

	std::shared_ptr<Transform> root_transform = current_entity->get_transform();
	global_transform->translation = root_transform->translation;
	global_transform->rotation = root_transform->rotation;
	global_transform->scale = root_transform->scale;

	for (int i = local_transforms.size() - 2; //The last transform index after the root
	i >= 0; 
	i--)
	{
		std::shared_ptr<Transform> local_transform = local_transforms.at(i);

		glm::vec3 radians_rotation = glm::vec3(
			glm::radians(global_transform->rotation.x),
			glm::radians(global_transform->rotation.y),
			glm::radians(global_transform->rotation.z)
		);

		glm::vec3 distance_vector = local_transform->translation - global_transform->translation;
		float distance = glm::length(distance_vector);
		glm::vec3 direction = glm::normalize(distance_vector);

		if (!(std::isnan(direction.x) || std::isnan(direction.y) || std::isnan(direction.z)))
		{
			direction = glm::rotateX(direction, radians_rotation.x);
			direction = glm::rotateY(direction, radians_rotation.y);
			direction = glm::rotateZ(direction, radians_rotation.z);
			global_transform->translation += direction * distance;
		}

		global_transform->rotation += local_transform->rotation;
		global_transform->scale *= local_transform->scale;
	}

	return global_transform;
}

std::vector<size_t> Omnific::SceneLayer::get_render_order_index_cache()
{
	return this->render_order_index_cache;
}

std::unordered_map<std::string, std::vector<size_t>> Omnific::SceneLayer::get_component_index_caches()
{
	return this->component_index_caches;
}

void Omnific::SceneLayer::clear_start_entity_queue()
{
	while (!this->start_entities_queue.empty())
		this->start_entities_queue.pop();
}

void Omnific::SceneLayer::clear_finish_entity_queue()
{
	while (!this->finish_entities_queue.empty())
		this->finish_entities_queue.pop();
}

std::queue<Omnific::EntityID> Omnific::SceneLayer::get_start_entity_queue()
{
	return this->start_entities_queue;
}

std::queue<Omnific::EntityID> Omnific::SceneLayer::get_finish_entity_queue()
{
	return this->finish_entities_queue;
}

std::vector<std::shared_ptr<Omnific::Component>> Omnific::SceneLayer::get_components()
{
	return this->components;
}

std::shared_ptr<Omnific::Entity> Omnific::SceneLayer::get_entity(EntityID entity_id)
{
	return this->entities.at(entity_id);
}

std::shared_ptr<Omnific::Entity> Omnific::SceneLayer::get_entity_by_name(std::string name)
{
	std::shared_ptr<Entity> entity(new Entity());

	for (auto it = this->entities.begin(); it != this->entities.end(); it++)
		if (it->second->name == name)
			return it->second;

	return entity;
}

std::shared_ptr<Omnific::Entity> Omnific::SceneLayer::get_last_entity()
{
	return this->entities.at(this->last_entity_id);
}

std::unordered_map<Omnific::EntityID, std::shared_ptr<Omnific::Entity>>& Omnific::SceneLayer::get_entities()
{
	return this->entities;
}

std::shared_ptr<Omnific::Component> Omnific::SceneLayer::get_component_by_id(ComponentID component_id)
{
	std::shared_ptr<Component> component;

	if (this->components_by_id.count(component_id) > 0)
	{
		component = this->components_by_id.at(component_id);
	}

	return component;
}

std::shared_ptr<Omnific::Component> Omnific::SceneLayer::get_component(std::string type, EntityID entity_id)
{
	std::shared_ptr<Entity> entity = this->get_entity(entity_id);
	std::shared_ptr<Component> component;

	if (entity->component_ids.count(type) > 0)
		component = this->get_component_by_id(entity->component_ids.at(type));

	return component;
}

Omnific::SceneLayerID Omnific::SceneLayer::get_id()
{
	return this->id;
}

std::string Omnific::SceneLayer::get_name()
{
	return this->name;
}