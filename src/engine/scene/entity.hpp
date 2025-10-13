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

#include <foundations/resources/shader.hpp>
#include "scene/components/component.hpp"
#include <foundations/aliases.hpp>
#include "foundations/constants.hpp"
#include "foundations/transform.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <foundations/singletons/uid_generator.hpp>
#include <memory>
#include <engine_api.hpp>

namespace Omnific
{
	/* An object that exists within the Scene with its own
	   identity, components and  hierarchy with other Entities. */
	class OMNIFIC_ENGINE_API Entity
	{
		friend class Scene;
	public:
		EntityID parent_id = 0;
		std::vector<EntityID> child_ids;
		bool is_2d = false;

		Entity()
		{
			this->id = UIDGenerator::get_new_uid();
			this->name = "Entity (ID:" + std::to_string(this->id) + ")";
			this->transform = std::shared_ptr<Transform>(new Transform());
		}

		Entity(std::string entity_name)
		{
			this->id = UIDGenerator::get_new_uid();
			this->transform = std::shared_ptr<Transform>(new Transform());
			this->name = entity_name;
		}

		/*Sets name publicly only when it is not attached to a Scene.*/
		void set_name(std::string name);
		std::string get_name();
		EntityID get_id();
		std::shared_ptr<Transform> get_transform();
		std::vector<std::string> get_tags();
		std::unordered_map<std::string, ComponentID> get_component_ids();
		ComponentID get_renderable_id();
	private:
		std::string name;
		EntityID id = 0;
		std::shared_ptr<Transform> transform;
		std::vector<std::string> tags;
		std::unordered_map<std::string, ComponentID> component_ids;
		ComponentID renderable_id = 0;

		bool is_attached_to_scene = false;
	};
}