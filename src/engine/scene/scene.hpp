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
#include <map>
#include <unordered_map>
#include <foundations/aliases.hpp>
#include <string>
#include <engine_api.hpp>

#include <foundations/resources/image.hpp>

#include "scene/components/component.hpp"
#include <scene/components/renderable.hpp>
#include "entity.hpp"
#include <tiny_gltf.h>
#include <foundations/clock.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Scene
	{
		friend class Engine;
	public:
		Scene();
		Scene(std::string filepath);

		void serialize_to(std::string filepath);
		void deserialize_from(std::string filepath);

		void add_entity(std::shared_ptr<Entity> entity);
		void add_empty_entity();
		void add_entity_to_parent_entity(std::shared_ptr<Entity> entity, EntityID parent_entity_id);
		void add_entity_to_parent_entity_by_name(std::shared_ptr<Entity> entity, std::string parent_entity_name);
		void merge_another_scene_to_parent_entity(std::shared_ptr<Scene> other_scene, EntityID parent_entity_id);
		void merge_another_scene_to_parent_entity_by_name(std::shared_ptr<Scene> other_scene, std::string parent_entity_name);
		void set_entity_name(EntityID entity_id, std::string name);
		void add_entity_tag(EntityID entity_id, std::string tag);
		void add_component(EntityID entity_id, std::shared_ptr<Component> component);
		void add_component_to_last_entity(std::shared_ptr<Component> component);

		void remove_entity(EntityID entity_id);
		void remove_component(EntityID entity_id, std::string type);

		std::shared_ptr<Transform> calculate_global_transform(EntityID local_transform_entity_id);

		void clear_start_entity_queue();
		void clear_finish_entity_queue();

		bool is_active();

		std::queue<EntityID> get_start_entity_queue();
		std::queue<EntityID> get_finish_entity_queue();

		std::vector<std::shared_ptr<Component>> get_components();
		std::vector<size_t> get_rendering_order_index_cache();
		std::unordered_map<std::string, std::vector<size_t>> get_component_index_caches();
		std::shared_ptr<Entity> get_entity(EntityID entity_id);
		std::shared_ptr<Entity> get_entity_by_name(std::string name);
		std::shared_ptr<Entity> get_last_entity();
		std::unordered_map<EntityID, std::shared_ptr<Entity>> get_entities();
		std::shared_ptr<Component> get_component_by_id(ComponentID component_id);
		std::shared_ptr<Component> get_component(std::string type, EntityID entity_id);
		std::vector<std::shared_ptr<Renderable>> get_renderables_in_rendering_order();

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

			for (size_t i = 0; i < component_index_count; ++i)
			{
				std::shared_ptr<T> casted_component = std::dynamic_pointer_cast<T>(components.at(component_indices.at(i)));
				if (casted_component != nullptr)
				{
					components_by_type.push_back(casted_component);
				}
			}

			return components_by_type;
		}

		template <class T>
		std::unordered_map<ComponentID, std::shared_ptr<T>> get_components_by_type_in_dictionary()
		{
			std::unordered_map<ComponentID, std::shared_ptr<T>> component_dictionary;
			std::vector<std::shared_ptr<T>> components_by_type = this->get_components_by_type<T>();

			for (int i = 0; i < components_by_type.size(); ++i)
			{
				std::shared_ptr<T> component = components_by_type.at(i);
				component_dictionary.emplace(component->get_id(), component);
			}

			return component_dictionary;
		}

		std::string get_name();
		SceneID get_id();
	private:
		SceneID id = 0;
		std::string name;

		std::shared_ptr<Clock> fps_monitor_clock;

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
		std::vector<size_t> rendering_order_index_cache;
		std::unordered_map<EntityID, std::shared_ptr<Transform>> cached_global_transforms;

		void load_from_gltf(std::string filepath);
		std::vector<uint8_t> read_gltf_buffer(std::vector<unsigned char> buffer_data, tinygltf::BufferView buffer_view);
		std::vector<float> read_gltf_primitive_attribute(tinygltf::Model model, std::string attribute_name, size_t index);
		std::vector<uint32_t> read_gltf_primitive_indices(tinygltf::Model model, size_t index);
		std::shared_ptr<Image> read_gltf_image(tinygltf::Model model, int texture_index);

		void update_debug_statistics();
	};
}