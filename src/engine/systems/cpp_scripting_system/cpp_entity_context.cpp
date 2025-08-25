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

#include "cpp_entity_context.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>

Omnific::CPPEntityContext* Omnific::CPPEntityContext::instance = nullptr;

void Omnific::CPPEntityContext::bind_entity(EntityID entity_id)
{
	get_instance()->bound_entity_id = entity_id;
}

void Omnific::CPPEntityContext::bind_time_delta(float time_delta)
{
	get_instance()->time_delta = time_delta;
}

bool Omnific::CPPEntityContext::has_component(std::string type)
{
	return get_instance()->get_scene()->get_entity(get_instance()->bound_entity_id)->get_component_ids().count(type) > 0;
}

std::shared_ptr<Omnific::Entity> Omnific::CPPEntityContext::get_entity()
{
	return get_instance()->get_scene()->get_entity(get_instance()->bound_entity_id);
}

std::shared_ptr<Omnific::Scene> Omnific::CPPEntityContext::get_scene()
{
	return SceneStorage::get_active_scene();
}

std::shared_ptr<Omnific::Component> Omnific::CPPEntityContext::get_component(std::string type)
{
	std::shared_ptr<Component> component(new Component());

	std::shared_ptr<Entity> entity = get_instance()->get_scene()->get_entity(get_instance()->bound_entity_id);
	std::vector<std::shared_ptr<Component>> components = get_instance()->get_scene()->get_components();

	for (int i = 0; i < components.size(); ++i)
		if (components.at(i)->get_id() == entity->get_component_ids().at(type))
			component = components.at(i);

	return component;
}

float Omnific::CPPEntityContext::get_time_delta()
{
	return get_instance()->time_delta;
}

Omnific::CPPEntityContext* Omnific::CPPEntityContext::get_instance()
{
	if (instance == nullptr)
		instance = new CPPEntityContext();
	return instance;
}