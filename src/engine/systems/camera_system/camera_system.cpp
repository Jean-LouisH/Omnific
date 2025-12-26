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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <foundations/singletons/platform/platform.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/renderable.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_storage.hpp>
#include <scene/components/viewport.hpp>
#include <foundations/singletons/configuration.hpp>
#include <math.h>

#define CAMERA_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME "camera_system_on_input_frame_time"
#define CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME "camera_system_on_update_frame_time"

Omnific::CameraSystem::~CameraSystem()
{
	this->finalize();
}

void Omnific::CameraSystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(CAMERA_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME, {"camera_system", "on_input_frame_time"});
	Profiler::add_clock(CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME, {"camera_system", "on_update_frame_time"});
	Platform::get_logger().write("Initialized Camera System");
}

void Omnific::CameraSystem::on_input()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CAMERA_SYSTEM_ON_INPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();

	std::shared_ptr<Entity> viewport_entity = scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME);
	if (viewport_entity != nullptr)
	{
		std::shared_ptr<Viewport> viewport = scene->get_component_by_type<Viewport>(viewport_entity->get_id());
		if (viewport != nullptr)
		{
			std::shared_ptr<Entity> camera_entity = scene->get_entity_by_name(viewport->get_camera_entity_name());
			if (camera_entity != nullptr)
			{
				std::shared_ptr<Camera> camera = scene->get_component_by_type<Camera>(camera_entity->get_id());
				if (camera->enable_flyby_mode_on_default_input)
				{
					Inputs& inputs = Platform::get_inputs();
					if (inputs.is_right_mouse_button_pressed())
						camera->controller_state = Camera::ControllerState::FLYBY;
					else
						camera->controller_state = Camera::ControllerState::NONE;
				}
			}
		}
	}

	frame_time_clock->set_end();
}

void Omnific::CameraSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(CAMERA_SYSTEM_ON_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->autofit_viewports_to_renderable_widths(scene);
	this->move_with_controller_states(scene);
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

void Omnific::CameraSystem::move_with_controller_states(std::shared_ptr<Scene> scene)
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
			std::shared_ptr<Transform> camera_global_transform = scene->calculate_global_transform(camera->get_entity_id());
			Inputs& inputs = Platform::get_inputs();

			glm::vec3 input_translation_vector = glm::vec3(
				(float)(inputs.is_pressed("d") - inputs.is_pressed("a")), 
				(float)(inputs.is_pressed("e") - inputs.is_pressed("q")), 
				(float)(inputs.is_pressed("s") - inputs.is_pressed("w"))
			);

			glm::vec2 mouse_motion_velocity = inputs.get_mouse_motion_velocity();
			float fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time / MS_IN_S;

			if (camera->controller_state == Camera::ControllerState::NONE) 
			{
				inputs.set_relative_mouse_mode(false);
			}
			else if (camera->controller_state == Camera::ControllerState::FLYBY)
			{
				inputs.set_relative_mouse_mode(true);
				camera->linear_velocity += inputs.get_mouse_wheel_velocity().y;
				
				if (glm::length(camera->linear_velocity) < 0)
				{
					camera->linear_velocity = glm::vec3(0.0);
				}
				else if (glm::length(camera->linear_velocity) > camera->flyby_mode.target_max_linear_speed)
				{
					camera->linear_velocity *= camera->flyby_mode.target_max_linear_speed / glm::length(camera->linear_velocity);
				}

				camera->angular_velocity = glm::vec3(camera->flyby_mode.target_max_angular_speed);
				camera_transform->rotate(glm::vec3(mouse_motion_velocity.y, mouse_motion_velocity.x, 0.0) * fixed_frame_time * -camera->angular_velocity);

				camera_transform->translation += (camera_transform->get_front_vector() * input_translation_vector.z + 
					camera_transform->get_right_vector() * input_translation_vector.x + 
					camera_transform->get_up_vector() * input_translation_vector.y) * 
					camera->linear_velocity * fixed_frame_time;
			}
			else if (camera->controller_state == Camera::ControllerState::FOLLOW)
			{
				inputs.set_relative_mouse_mode(false); 

				std::shared_ptr<Entity> follow_entity = scene->get_entity_by_name(camera->follow_mode.follow_target_entity);

				if (follow_entity != nullptr)
				{
					std::shared_ptr<Transform> follow_entity_global_transform = scene->calculate_global_transform(follow_entity->get_id());

					glm::vec2 input_rotation_vector = glm::vec2(
						inputs.get_action_axis("right_axis_x"),
						inputs.get_action_axis("right_axis_y")
					);

					//Rotate around Entity
					float max_rotate_around_speed_radians = glm::radians(camera->follow_mode.max_rotate_around_speed);
					float max_rotate_around_acceleration_radians = glm::radians(camera->follow_mode.max_rotate_around_acceleration);
					float rotate_around_deceleration_radians = glm::radians(camera->follow_mode.rotate_around_deceleration);
					glm::vec2 inversion = glm::vec2(1.0);
					glm::vec2 target_rotate_around_velocity = glm::vec2(0.0);
					glm::vec2 target_rotate_around_acceleration = glm::vec2(0.0);

					if (camera->is_x_rotation_inverted)
						inversion.x = -1.0;
					if (camera->is_y_rotation_inverted)
						inversion.y = -1.0;

					target_rotate_around_velocity += glm::vec2(1.0) * max_rotate_around_speed_radians * input_rotation_vector * inversion;
					target_rotate_around_acceleration += glm::vec2(1.0) * max_rotate_around_acceleration_radians * input_rotation_vector * inversion;
					camera->rotate_around_velocity += target_rotate_around_acceleration * fixed_frame_time;
					glm::vec3 camera_rotations_in_degrees = camera_transform->get_rotation_in_degrees_euler_angles();

					if (glm::length(camera->rotate_around_velocity) > glm::length(target_rotate_around_velocity) && !(abs(input_rotation_vector.x) < 0.1))
					{
						camera->rotate_around_velocity *= glm::length(target_rotate_around_velocity) / glm::length(camera->rotate_around_velocity);

						if (camera->rotate_around_velocity.x > 0.0 && (camera->is_y_rotation_limited && camera_rotations_in_degrees.y > camera->rotation_upper_limits.y))
							camera->rotate_around_velocity.x = 0.0;
						else if (camera->rotate_around_velocity.x < 0.0 && (camera->is_y_rotation_limited && camera_rotations_in_degrees.y < camera->rotation_lower_limits.y))
							camera->rotate_around_velocity.x = 0.0;
					}
					else if (abs(input_rotation_vector.x) < 0.1)
					{
						if (camera->rotate_around_velocity.x > 0.0)
						{
							camera->rotate_around_velocity -=  glm::vec2(1.0, 0.0) * rotate_around_deceleration_radians * fixed_frame_time;
							if (camera->rotate_around_velocity.x < 0.0)
								camera->rotate_around_velocity.x = 0.0;
						}
						else
						{
							camera->rotate_around_velocity +=  glm::vec2(1.0, 0.0) * rotate_around_deceleration_radians * fixed_frame_time;
							if (camera->rotate_around_velocity.x > 0.0)
								camera->rotate_around_velocity.x = 0.0;
						}
					}

					if (glm::length(camera->rotate_around_velocity) > glm::length(target_rotate_around_velocity) && !(abs(input_rotation_vector.y) < 0.1))
					{
						camera->rotate_around_velocity *= glm::length(target_rotate_around_velocity) / glm::length(camera->rotate_around_velocity);

						if (camera->rotate_around_velocity.y > 0.0 && (camera->is_x_rotation_limited && camera_rotations_in_degrees.x > camera->rotation_upper_limits.x))
							camera->rotate_around_velocity.y = 0.0;
						else if (camera->rotate_around_velocity.y < 0.0 && (camera->is_x_rotation_limited && camera_rotations_in_degrees.x < camera->rotation_lower_limits.x))
							camera->rotate_around_velocity.y = 0.0;
					}
					else if (abs(input_rotation_vector.y) < 0.1)
					{
						if (camera->rotate_around_velocity.y > 0.0)
						{
							camera->rotate_around_velocity -=  glm::vec2(0.0, 1.0) * rotate_around_deceleration_radians * fixed_frame_time;
							if (camera->rotate_around_velocity.y < 0.0)
								camera->rotate_around_velocity.y = 0.0;
						}
						else
						{
							camera->rotate_around_velocity +=  glm::vec2(0.0, 1.0) * rotate_around_deceleration_radians * fixed_frame_time;
							if (camera->rotate_around_velocity.y > 0.0)
								camera->rotate_around_velocity.y = 0.0;
						}
					}

					camera_transform->rotate_y_around(follow_entity_global_transform->translation, glm::degrees(camera->rotate_around_velocity.x) * fixed_frame_time);
					camera->follow_mode.follow_distances = glm::rotateY(camera->follow_mode.follow_distances, camera->rotate_around_velocity.x * fixed_frame_time);
					// camera_transform->rotate_x_around(follow_entity_global_transform->translation, glm::degrees(camera->rotate_around_velocity.y) * fixed_frame_time);
					// camera->follow_mode.follow_distances = glm::rotateX(camera->follow_mode.follow_distances, camera->rotate_around_velocity.y * fixed_frame_time);

					//Translate with Entity
					camera_transform->interpolate_with_translation(follow_entity_global_transform->translation + camera->follow_mode.follow_distances, camera->follow_mode.interpolation_speed * fixed_frame_time);

					std::shared_ptr<Transform> camera_global_transform = scene->calculate_global_transform(camera->get_entity_id());
					camera_transform->look_at(glm::normalize(camera_global_transform->translation - follow_entity_global_transform->translation), glm::vec3(0.0, 1.0, 0.0));
				}
			}
			else if (camera->controller_state == Camera::ControllerState::FOLLOW_GROUP)
			{
				inputs.set_relative_mouse_mode(false);
			}
			else if (camera->controller_state == Camera::ControllerState::CINEMATIC)
			{
				inputs.set_relative_mouse_mode(false);
			}
		}
	}
}