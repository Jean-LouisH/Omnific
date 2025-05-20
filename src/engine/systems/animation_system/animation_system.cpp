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
#include "scene/scene.hpp"
#include <foundations/singletons/configuration.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include <scene/components/sprite.hpp>

#define ANIMATION_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "animation_system_on_fixed_update_frame_time"

Omnific::AnimationSystem::~AnimationSystem()
{
	this->finalize();
}

void Omnific::AnimationSystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(ANIMATION_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"animation_system", "on_fixed_update_frame_time"});
	Platform::get_logger().write("Initialized Animation System");
}

void Omnific::AnimationSystem::on_fixed_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(ANIMATION_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	for (const auto& [id, scene_layer] : scene->get_scene_layers())
	{
		this->update_sprites(scene_layer);
	}
	frame_time_clock->set_end();
}

void Omnific::AnimationSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::AnimationSystem::update_sprites(std::shared_ptr<SceneLayer> scene_layer)
{
	const uint32_t ms_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time;
	std::vector<std::shared_ptr<Sprite>> sprite_containers = scene_layer->get_components_by_type<Sprite>();

	for (size_t i = 0; i < sprite_containers.size(); ++i)
	{
		sprite_containers.at(i)->update(ms_per_fixed_update * 1.0 / MS_IN_S);
	}
}