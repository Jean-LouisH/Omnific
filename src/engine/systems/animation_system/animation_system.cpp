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
#include <scene/components/animator.hpp>
#include <foundations/singletons/scene_manager.hpp>

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
	std::shared_ptr<Scene> scene = SceneManager::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(ANIMATION_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	this->animate_sprites(scene);
	this->animate_camera_relative_movements(scene);
	this->animate_jump_movements(scene);
	this->animate_skeletons_and_properties(scene);
	frame_time_clock->set_end();
}

void Omnific::AnimationSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::AnimationSystem::animate_sprites(std::shared_ptr<Scene> scene)
{
	const uint32_t ms_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time;
	std::vector<std::shared_ptr<Sprite>> sprites = scene->get_components_by_type<Sprite>();

	for (size_t i = 0; i < sprites.size(); ++i)
	{
		sprites.at(i)->update(ms_per_fixed_update * 1.0 / MS_IN_S);
	}
}

void Omnific::AnimationSystem::animate_camera_relative_movements(std::shared_ptr<Scene> scene)
{
	float fixed_frame_time = Configuration::get_instance()->performance_settings.fixed_frame_time / MS_IN_S;
	std::vector<std::shared_ptr<CameraRelativeMovement>> camera_relative_movements = scene->get_components_by_type<CameraRelativeMovement>();
	std::shared_ptr<Entity> default_viewport_entity = scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME);

	if (default_viewport_entity == nullptr)
		return;

	std::shared_ptr<Viewport> default_viewport = scene->get_component_by_type<Viewport>(default_viewport_entity->get_id());

	if (default_viewport == nullptr)
		return;

	std::shared_ptr<Entity> current_camera_entity = scene->get_entity_by_name(default_viewport->get_camera_entity_name());

	if (current_camera_entity == nullptr)
		return;

	std::shared_ptr<Transform> current_camera_transform = current_camera_entity->get_transform();

	if (current_camera_transform != nullptr)
	{
		Inputs& inputs = Platform::get_inputs();
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

void Omnific::AnimationSystem::animate_jump_movements(std::shared_ptr<Scene> scene)
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

void Omnific::AnimationSystem::animate_skeletons_and_properties(std::shared_ptr<Scene> scene)
{
	const uint32_t ms_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time;
    std::vector<std::shared_ptr<Animator>> animators = scene->get_components_by_type<Animator>();

    for (std::shared_ptr<Animator>& animator : animators)
    {
        //Skeletal Animation
        for (auto& [animation_name, skeletal_animation] : animator->skeletal_animations)
        {
            if (!skeletal_animation->is_playing || skeletal_animation->repeat_count >= skeletal_animation->allowable_repeats)
                continue;

            skeletal_animation->progress += (ms_per_fixed_update * 1.0 / MS_IN_S) * skeletal_animation->playback_speed_percentage;

            if (skeletal_animation->progress > skeletal_animation->duration)
            {
                skeletal_animation->repeat_count++;
                if (skeletal_animation->repeat_count < skeletal_animation->allowable_repeats)
                {
                    // Loop back around (modulo remainder preserves timing precision)
                    skeletal_animation->progress = std::fmod(skeletal_animation->progress, skeletal_animation->duration);
                }
                else
                {
                    skeletal_animation->progress = skeletal_animation->duration;
                    continue;
                }
            }

            for (const auto& channel : skeletal_animation->channels)
            {
                std::shared_ptr<Entity> entity = scene->get_entity(channel.target_entity_id);
                if (!entity) continue; 

                const auto& sampler = skeletal_animation->samplers[channel.sampler_index];
                if (sampler.input_timestamps.empty()) continue;

                size_t next_index = 0;
                while (next_index < sampler.input_timestamps.size() && skeletal_animation->progress > sampler.input_timestamps[next_index])
                {
                    next_index++;
                }

                size_t previous_index = (next_index == 0) ? 0 : next_index - 1;
                if (next_index >= sampler.input_timestamps.size()) 
                {
                    next_index = previous_index; 
                }

                float blend_factor = 0.0f;
                if (previous_index != next_index)
                {
                    float prev_time = sampler.input_timestamps[previous_index];
                    float next_time = sampler.input_timestamps[next_index];
                    blend_factor = (skeletal_animation->progress - prev_time) / (next_time - prev_time);
                }

                if (sampler.interpolation == SkeletalAnimation::InterpolationType::STEP)
                {
                    blend_factor = 0.0f; // Lock 100% to previous frame until next index boundaries trigger
                }

                std::shared_ptr<Transform> transform = entity->get_transform();
                if (!transform) continue;

                if (channel.path == SkeletalAnimation::Path::TRANSLATION)
                {
                    glm::vec3 start_translation = glm::vec3(sampler.output_values[previous_index]);
                    glm::vec3 end_translation = glm::vec3(sampler.output_values[next_index]);
                    transform->translation = glm::mix(start_translation, end_translation, blend_factor);
                }
                else if (channel.path == SkeletalAnimation::Path::ROTATION)
                {
				
					glm::vec4 start_rotation = sampler.output_values[previous_index];
					glm::vec4 end_rotation = sampler.output_values[next_index];
					glm::quat start_rotation_quat(start_rotation.w, start_rotation.x, start_rotation.y, start_rotation.z);
					glm::quat end_rotation_quat(end_rotation.w, end_rotation.x, end_rotation.y, end_rotation.z);
                    transform->rotation = glm::slerp(start_rotation_quat, end_rotation_quat, blend_factor);
                }
                else if (channel.path == SkeletalAnimation::Path::SCALE)
                {
                    glm::vec3 start_scale = glm::vec3(sampler.output_values[previous_index]);
                    glm::vec3 end_scale = glm::vec3(sampler.output_values[next_index]);
                    transform->scale = glm::mix(start_scale, end_scale, blend_factor);
                }
            }
        }

        // TODO: Property Animation
    }
}