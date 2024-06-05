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

#include "scene/components/component.hpp"
#include <foundations/singletons/uid_generator.hpp>

Omnia::Component::Component()
{
	this->type = TYPE_STRING;
}

void Omnia::Component::set_entity_id(EntityID entity_id)
{
	this->entity_id = entity_id;
}

void Omnia::Component::set_component_hierarchy(std::vector<std::shared_ptr<Component>> component_hierarchy)
{
	this->component_hierarchy = component_hierarchy;
}

void Omnia::Component::deserialize(YAML::Node yaml_node)
{

}

Omnia::EntityID Omnia::Component::get_entity_id()
{
	return this->entity_id;
}

bool Omnia::Component::is_attached_to_entity()
{
	return this->get_entity_id() != 0;
}

bool Omnia::Component::is_renderable()
{
	return false;
}