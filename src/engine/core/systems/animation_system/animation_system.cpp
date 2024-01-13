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

#include "animation_system.hpp"
#include "core/scene/scene.hpp"
#include <core/singletons/configuration.hpp>
#include <core/singletons/os/os.hpp>

#include <core/components/sprite.hpp>

Omnia::AnimationSystem::~AnimationSystem()
{
	this->finalize();
}

void Omnia::AnimationSystem::initialize()
{
	this->is_initialized = true;
	OS::get_logger().write("Initialized Animation System");
}

void Omnia::AnimationSystem::on_compute(std::shared_ptr<Scene> scene)
{
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		this->update_sprite_containers(scene_layer_it.second);
	}
}

void Omnia::AnimationSystem::finalize()
{
	this->is_initialized = false;
}

void Omnia::AnimationSystem::update_sprite_containers(std::shared_ptr<SceneLayer> scene_layer)
{
	const uint32_t ms_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update;
	std::vector<std::shared_ptr<Sprite>> sprite_containers = scene_layer->get_components_by_type<Sprite>();

	for (size_t i = 0; i < sprite_containers.size(); i++)
	{
		sprite_containers.at(i)->update(ms_per_compute_update * 1.0 / MS_IN_S);
	}
}