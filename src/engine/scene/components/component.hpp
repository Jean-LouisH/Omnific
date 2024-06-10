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


#include <foundations/aliases.hpp>
#include <foundations/constants.hpp>
#include <foundations/resources/image.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <engine_api.hpp>
#include <yaml-cpp/yaml.h>
#include <foundations/registerable.hpp>

namespace Omnia
{
	/* Base class for objects that store and manipulate 
	   data relevant to individual Entities in a Scene.
	   These are attached to Entities when added to a Scene.*/
    class OMNIA_ENGINE_API Component : public Registerable
    {
	public:
		static constexpr const char* TYPE_STRING = "Component";

		Component();

		void set_entity_id(EntityID entity_id);
		void set_component_hierarchy(std::vector<std::shared_ptr<Component>> component_hierarchy);
		virtual void deserialize(YAML::Node yaml_node);

		EntityID get_entity_id();
		bool is_attached_to_entity();
		virtual bool is_renderable();
	protected:
		std::vector<std::shared_ptr<Component>> component_hierarchy;
		std::unordered_map<std::string, std::string> properties_with_types; //<PropertyName, PropertyType>
	private:
		EntityID entity_id = 0;
    };
}