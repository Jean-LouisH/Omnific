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

#include "systems/system.hpp"
#include <foundations/singletons/event_bus.hpp>

Omnia::System::~System()
{
	this->is_initialized = false;
}

void Omnia::System::initialize()
{

}

void Omnia::System::on_start(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_input(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_early(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_logic(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_compute(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_late(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::on_finish(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::finalize()
{

}

bool Omnia::System::get_is_initialized()
{
	return this->is_initialized;
}

bool Omnia::System::has_scene_changed(std::shared_ptr<Scene> scene)
{
	bool active_scene_changed = this->active_scene_id != scene->get_id();

	if (active_scene_changed)
		this->active_scene_id = scene->get_id();

	return (EventBus::query_count(OMNIA_EVENT_COMPONENT_ADDED) ||
		EventBus::query_count(OMNIA_EVENT_COMPONENT_REMOVED) ||
		active_scene_changed);
}