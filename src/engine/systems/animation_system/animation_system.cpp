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
#include <foundations/constants.hpp>
#include <foundations/singletons/configuration.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include <scene/components/sprite.hpp>
#include <scene/components/camera_relative_movement.hpp>
#include <scene/components/jump_movement.hpp>
#include <scene/components/physics_body.hpp>
#include <scene/components/collider.hpp>
#include <scene/components/viewport.hpp>
#include <scene/components/camera.hpp>
#include <foundations/singletons/scene_storage.hpp>

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

void Omnific::AnimationSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(ANIMATION_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->update_sprites(scene);
	this->execute_camera_relative_movements(scene);
	this->execute_jump_movements(scene);
	frame_time_clock->set_end();
}

void Omnific::AnimationSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::AnimationSystem::update_sprites(std::shared_ptr<Scene> scene)
{
	const uint32_t ms_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time;
	std::vector<std::shared_ptr<Sprite>> sprites = scene->get_components_by_type<Sprite>();

	for (size_t i = 0; i < sprites.size(); ++i)
	{
		sprites.at(i)->update(ms_per_fixed_update * 1.0 / MS_IN_S);
	}
}

void Omnific::AnimationSystem::execute_camera_relative_movements(std::shared_ptr<Scene> scene)
{
	float fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time / MS_IN_S;
	std::vector<std::shared_ptr<CameraRelativeMovement>> camera_relative_movements = scene->get_components_by_type<CameraRelativeMovement>();
	std::shared_ptr<Viewport> default_viewport = scene->get_component_by_type<Viewport>(scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME)->get_id());
	std::shared_ptr<Transform> current_camera_transform = scene->get_entity_by_name(default_viewport->get_camera_entity_name())->get_transform();
	Inputs& inputs = Platform::get_inputs();

	if (current_camera_transform != nullptr)
	{
		for (size_t i = 0; i < camera_relative_movements.size(); ++i)
		{
			std::shared_ptr<CameraRelativeMovement> camera_relative_movement = camera_relative_movements.at(i);
			std::shared_ptr<Transform> entity_transform = scene->get_entity(camera_relative_movement->get_entity_id())->get_transform();
			glm::vec2 input_vector = glm::vec2(inputs.get_action_axis("left_axis_x"), -inputs.get_action_axis("left_axis_y"));
			float deadzone = 0.1;
			glm::vec3 camera_forward = current_camera_transform->get_front_vector();
			glm::vec3 camera_right = current_camera_transform->get_right_vector();

			camera_forward.y = 0.0;
			camera_right.y = 0.0;

			camera_forward = glm::normalize(camera_forward);
			camera_right = glm::normalize(camera_right);

			if (abs(input_vector.x) < deadzone && abs(input_vector.y) < deadzone)
			{
				camera_relative_movement->linear_velocity = glm::vec3(0.0);
			}
			else
			{
				camera_relative_movement->linear_velocity += (camera_forward * input_vector.y + 
							camera_right * input_vector.x) * camera_relative_movement->max_acceleration * fixed_frame_time;

				if (glm::length(camera_relative_movement->linear_velocity) > camera_relative_movement->max_movement_speed)
					camera_relative_movement->linear_velocity *= camera_relative_movement->max_movement_speed / glm::length(camera_relative_movement->linear_velocity);

				entity_transform->look_at(camera_relative_movement->linear_velocity, glm::vec3(0.0, 1.0, 0.0));
			}

			entity_transform->translation += camera_relative_movement->linear_velocity * fixed_frame_time;
		}
	}
}

void Omnific::AnimationSystem::execute_jump_movements(std::shared_ptr<Scene> scene)
{
	float fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time / MS_IN_S;
	std::vector<std::shared_ptr<JumpMovement>> jump_movements = scene->get_components_by_type<JumpMovement>();
	Inputs& inputs = Platform::get_inputs();

	for (size_t i = 0; i < jump_movements.size(); ++i)
	{
		std::shared_ptr<JumpMovement> jump_movement = jump_movements.at(i);
		std::shared_ptr<PhysicsBody> physics_body = scene->get_component_by_type<PhysicsBody>(jump_movement->get_entity_id());
		std::shared_ptr<Collider> collider = scene->get_component_by_type<Collider>(jump_movement->get_entity_id());

		if (physics_body != nullptr && collider != nullptr)
		{
			std::shared_ptr<Transform> entity_global_transform = scene->calculate_global_transform(jump_movement->get_entity_id());
			float global_y_position = entity_global_transform->translation.y;
			float extra_free_fall_height = (((jump_movement->jump_start_height + jump_movement->max_jump_height) + pow(jump_movement->jump_speed, 2)) / 
				(2 * -(physics_body->gravity * physics_body->gravity_scale)));

			if (collider->is_on_floor())
			{
				jump_movement->jumps = jump_movement->max_jump_count;
			}

			if (inputs.is_on_press("k") || inputs.is_pressed("k"))
			{
				if (inputs.is_on_press("k") && jump_movement->jumps > 0)
				{
					jump_movement->jump_start_height = global_y_position;
					jump_movement->jumps -= 1;
					physics_body->linear_velocity.y += jump_movement->jump_speed;
				}

				if (!collider->is_on_ceiling() && (physics_body->linear_velocity.y > 0 && 
					(global_y_position < jump_movement->jump_start_height + jump_movement->max_jump_height - extra_free_fall_height)))
				{
					physics_body->linear_velocity.y += jump_movement->jump_speed;
				}
			}
		}
	}
}