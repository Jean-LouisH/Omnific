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
#include <scene/components/renderable.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_storage.hpp>
#include <scene/components/viewport.hpp>
#include <foundations/singletons/configuration.hpp>

#define CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "camera_system_on_update_frame_time"

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

void Omnific::CameraSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->autofit_viewports_to_renderable_widths(scene);
	this->move_with_controller_state(scene);
	frame_time_clock->set_end();
}

void Omnific::CameraSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::CameraSystem::autofit_viewports_to_renderable_widths(std::shared_ptr<Scene> scene)
{
	for (std::shared_ptr<Camera>& camera: scene->get_components_by_type<Camera>())
	{
		std::shared_ptr<Entity> target_entity = scene->get_entity_by_name(camera->viewport_target_entity);
		if (target_entity != nullptr)
		{
			std::shared_ptr<Renderable> renderable = std::dynamic_pointer_cast<Renderable>(scene->get_component_by_id(target_entity->get_renderable_id()));
			if (renderable != nullptr)
			{
				camera->set_viewport_width(renderable->get_dimensions().x);
			}	
		}
	}
}

void Omnific::CameraSystem::move_with_controller_state(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Entity> viewport_entity = scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME);
	if (viewport_entity != nullptr)
	{
		std::shared_ptr<Viewport> viewport = scene->get_component_by_type<Viewport>(viewport_entity->get_id());
		if (viewport != nullptr)
		{
			std::shared_ptr<Entity> camera_entity = scene->get_entity_by_name(viewport->get_camera_entity_name());
			std::shared_ptr<Camera> camera = scene->get_component_by_type<Camera>(camera_entity->get_id());
			std::shared_ptr<Transform> camera_transform = camera_entity->get_transform();
			Inputs& inputs = Platform::get_inputs();
			glm::vec2 mouse_motion_velocity = inputs.get_mouse_motion_velocity();
			float fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time / MS_IN_S;

			switch (camera->controller_state)
			{
				case Camera::ControllerState::FLYBY: 

					camera->flyby_translation_speed += inputs.get_mouse_wheel_velocity().y;
					
					if (camera->flyby_translation_speed < 0)
					{
						camera->flyby_translation_speed  = 0;
					}

					camera_transform->rotation -= glm::vec3(mouse_motion_velocity.y, mouse_motion_velocity.x, 0.0) * fixed_frame_time * camera->flyby_rotation_speed;

					camera_transform->translation += (camera_transform->get_front_vector() * (float)(inputs.is_pressed("s") - inputs.is_pressed("w")) + 
						camera_transform->get_right_vector() * (float)(inputs.is_pressed("d") - inputs.is_pressed("a")) + 
						camera_transform->get_up_vector() * (float)(inputs.is_pressed("e") - inputs.is_pressed("q"))) * 
						camera->flyby_translation_speed * fixed_frame_time;
				break;
				case Camera::ControllerState::FOLLOW: ; break;
				case Camera::ControllerState::FOLLOW_GROUP: ; break;
				case Camera::ControllerState::CINEMATIC: ; break;
			}
		}
	}
}