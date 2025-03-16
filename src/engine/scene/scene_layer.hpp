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

#include <vector>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_map>
#include "scene/components/component.hpp"
#include "entity.hpp"
#include <unordered_map>
#include "foundations/aliases.hpp"
#include <string>

#include <engine_api.hpp>

#include <memory>

namespace Omnific
{
	class OMNIFIC_ENGINE_API SceneLayer
	{
	public:
		bool is_2d = false;
		std::string name;

		SceneLayer();

		void add_entity(std::shared_ptr<Entity> entity);
		void add_empty_entity();
		void set_entity_name(EntityID entity_id, std::string name);
		void add_entity_tag(EntityID entity_id, std::string tag);
		void add_component(EntityID entity_id, std::shared_ptr<Component> component);
		void add_component_to_last_entity(std::shared_ptr<Component> component);

		void remove_entity(EntityID entity_id);
		void remove_component(EntityID entity_id, std::string type);

		std::shared_ptr<Transform> compute_global_transform(EntityID local_transform_entity_id);

		void clear_start_entity_queue();
		void clear_finish_entity_queue();

		std::queue<EntityID> get_start_entity_queue();
		std::queue<EntityID> get_finish_entity_queue();

		std::vector<std::shared_ptr<Component>> get_components();
		std::vector<size_t> get_render_order_index_cache();
		std::unordered_map<std::string, std::vector<size_t>> get_component_index_caches();
		std::shared_ptr<Entity> get_entity(EntityID entity_id);
		std::shared_ptr<Entity> get_entity_by_name(std::string name);
		std::shared_ptr<Entity> get_last_entity();
		std::unordered_map<EntityID, std::shared_ptr<Entity>>& get_entities();
		std::shared_ptr<Component> get_component_by_id(ComponentID component_id);
		std::shared_ptr<Component> get_component(std::string type, EntityID entity_id);
		SceneLayerID get_id();
		std::string get_name();

		template <class T>
		std::shared_ptr<T> get_component_by_type(EntityID entity_id)
		{
			return std::dynamic_pointer_cast<T>(this->get_component(T::TYPE_STRING, entity_id));
		}

		template <class T>
		std::vector<std::shared_ptr<T>> get_components_by_type()
		{
			std::vector<std::shared_ptr<T>> components_by_type;
			std::vector<std::shared_ptr<Component>> components = this->get_components();
			std::vector<size_t> component_indices;
			std::unordered_map<std::string, std::vector<size_t>> component_index_caches = this->get_component_index_caches();

			if (component_index_caches.count(T::TYPE_STRING))
				component_indices = component_index_caches.at(T::TYPE_STRING);

			size_t component_index_count = component_indices.size();

			for (size_t i = 0; i < component_index_count; i++)
				components_by_type.push_back(std::dynamic_pointer_cast<T>(components.at(component_indices.at(i))));

			return components_by_type;
		}
	private:
		SceneLayerID id = 0;

		/*Entities are stored in maps for fast random access
		when Components invoke changes in other Components
		attached to the Entity.*/
		std::unordered_map<EntityID, std::shared_ptr<Entity>> entities;
		std::unordered_map<std::string, EntityID> entity_names;
		std::unordered_map<std::string, EntityID> entity_tags;

		/*Components are stored in vectors for fast linear access
		in engine system process loops.*/
		std::vector<std::shared_ptr<Component>> components;
		std::unordered_map<ComponentID, std::shared_ptr<Component>> components_by_id;

		std::queue<EntityID> start_entities_queue;
		std::queue<EntityID> finish_entities_queue;

		EntityID last_entity_id = 0;

		std::unordered_map<std::string, std::vector<size_t>> component_index_caches;
		std::vector<size_t> render_order_index_cache;
	};
}