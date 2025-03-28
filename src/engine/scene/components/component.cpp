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

Omnific::Component::Component()
{
	this->type = TYPE_STRING;
}

void Omnific::Component::deserialize(YAML::Node yaml_node)
{

}

Omnific::SceneLayerID Omnific::Component::get_scene_layer_id()
{
	return this->scene_layer_id;
}

Omnific::EntityID Omnific::Component::get_entity_id()
{
	return this->entity_id;
}

std::string Omnific::Component::get_entity_name()
{
	return this->entity_name;
}

bool Omnific::Component::is_attached_to_entity()
{
	return this->get_entity_id() != 0;
}

bool Omnific::Component::is_renderable()
{
	return false;
}