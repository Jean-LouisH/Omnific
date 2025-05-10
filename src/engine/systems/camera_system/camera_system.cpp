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

#include "camera_system.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/model.hpp>
#include <foundations/singletons/profiler.hpp>

#define CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "camera_system_on_update_frame_time_clock"

Omnific::CameraSystem::~CameraSystem()
{
	this->finalize();
}

void Omnific::CameraSystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"camera_system", "on_update_frame_time"});
	Platform::get_logger().write("Initialized Camera System");
}

void Omnific::CameraSystem::on_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		this->autofit_viewports_to_model_widths(scene_layer_it.second);
	}
	frame_time_clock->set_end();
}

void Omnific::CameraSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::CameraSystem::autofit_viewports_to_model_widths(std::shared_ptr<SceneLayer> scene_layer)
{
	for (std::shared_ptr<Camera> camera: scene_layer->get_components_by_type<Camera>())
	{
		std::shared_ptr<Entity> target_entity = scene_layer->get_entity_by_name(camera->viewport_target_entity);
		if (target_entity != nullptr)
		{
			std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(scene_layer->get_component_by_id(target_entity->get_model_id()));
			if (model != nullptr)
			{
				camera->set_viewport_width(model->get_dimensions().x);
			}	
		}
	}
}