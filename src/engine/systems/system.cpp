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

Omnific::System::~System()
{
	this->is_initialized = false;
}

void Omnific::System::initialize()
{

}

void Omnific::System::on_input()
{

}

void Omnific::System::on_entity_start()
{

}

void Omnific::System::on_early_update()
{

}

void Omnific::System::on_update()
{

}

void Omnific::System::on_fixed_update()
{

}

void Omnific::System::on_late_update()
{

}

void Omnific::System::on_entity_finish()
{

}

void Omnific::System::on_output()
{

}

void Omnific::System::finalize()
{

}

bool Omnific::System::get_is_initialized()
{
	return this->is_initialized;
}

bool Omnific::System::has_scene_changed()
{
	return EventBus::query_event_count(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED);
}

bool Omnific::System::have_components_changed()
{
	return EventBus::query_event_count(OMNIFIC_EVENT_SCENE_MODIFIED) || this->has_scene_changed();
}