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

#include "physics_system.hpp"
#include "scene/scene.hpp"
#include <foundations/constants.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/configuration.hpp>

#include <scene/components/collider.hpp>
#include <scene/components/physics_body.hpp>
#include <scene/components/timer.hpp>
#include <foundations/transform.hpp>

#include <foundations/singletons/event_bus.hpp>
#include <foundations/singletons/profiler.hpp>

#define PHYSICS_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "physics_system_on_fixed_update_frame_time_clock"

Omnific::PhysicsSystem::~PhysicsSystem()
{
	this->finalize();
}

void Omnific::PhysicsSystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(PHYSICS_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"physics_system", "on_fixed_update_frame_time"});
	Platform::get_logger().write("Initialized Physics System");
}

void Omnific::PhysicsSystem::on_fixed_update(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PHYSICS_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::shared_ptr<SceneLayer> scene_layer = scene_layer_it.second;
		this->update_timers(scene_layer);
		this->gravitate(scene_layer);
		this->decelerate(scene_layer);
		this->detect_collisions(scene_layer);
		this->handle_collisions_and_displacements(scene_layer);
	}
	frame_time_clock->set_end();
}

void Omnific::PhysicsSystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::PhysicsSystem::update_timers(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time * (1.0 / MS_IN_S);

	for (std::shared_ptr<Timer> timer : scene_layer->get_components_by_type<Timer>())
		timer->update(seconds_per_fixed_update);
}

void Omnific::PhysicsSystem::gravitate(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
		if (physics_body->has_gravity)
			physics_body->linear_velocity.y -= physics_body->gravity_scale * EARTH_GRAVITY * seconds_per_fixed_update;
}

void Omnific::PhysicsSystem::decelerate(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
	{
		if (physics_body->has_gravity)
		{
			physics_body->linear_velocity.x *= pow(physics_body->drag_ratio.x, seconds_per_fixed_update);
			physics_body->linear_velocity.y *= pow(physics_body->drag_ratio.y, seconds_per_fixed_update);
			physics_body->linear_velocity.z *= pow(physics_body->drag_ratio.z, seconds_per_fixed_update);
		}
	}
}

void Omnific::PhysicsSystem::detect_collisions(std::shared_ptr<SceneLayer> scene_layer)
{
	/* Very basic collision detection on boxes for now. */
	std::vector<std::shared_ptr<Collider>> colliders = scene_layer->get_components_by_type<Collider>();
	size_t colliders_count = colliders.size();
	std::string collision_event_string;

	for (size_t i = 0; i < colliders_count; i++)
	{
		std::shared_ptr<Collider> collider1 = colliders.at(i);
		std::shared_ptr<Transform> transform1 = scene_layer->get_entity(collider1->get_entity_id())->get_transform();
		glm::vec3 translation1 = transform1->translation;
		glm::vec3 scale1 = transform1->scale;
		scale1 *= 0.5;
		AABB3D aabb1 = collider1->box.aabb;

		for (size_t j = 0; j < colliders_count; j++)
		{
			if (i != j)
			{
				std::shared_ptr<Collider> collider2 = colliders.at(j);
				std::shared_ptr<Transform> transform2 = scene_layer->get_entity(collider2->get_entity_id())->get_transform();
				glm::vec3 translation2 = transform2->translation;
				glm::vec3 scale2 = transform2->scale;
				scale2 *= 0.5;
				AABB3D aabb2 = collider2->box.aabb;

				float box1left = translation1.x + aabb1.min.x * scale1.x;
				float box1right = translation1.x + aabb1.max.x * scale1.x;
				float box1top = translation1.y + aabb1.max.y * scale1.y;
				float box1bottom = translation1.y + aabb1.min.y * scale1.y;
				float box1front = translation1.z + aabb1.max.z * scale1.z;
				float box1back = translation1.z + aabb1.min.z * scale1.z;

				float box2left = translation2.x + aabb2.min.x * scale2.x;
				float box2right = translation2.x + aabb2.max.x * scale2.x;
				float box2top = translation2.y + aabb2.max.y * scale2.y;
				float box2bottom = translation2.y + aabb2.min.y * scale2.y;
				float box2front = translation2.z + aabb2.max.z * scale2.z;
				float box2back = translation2.z + aabb2.min.z * scale2.z;

				std::shared_ptr<Entity> entity1 = scene_layer->get_entity(collider1->get_entity_id());
				std::shared_ptr<Entity> entity2 = scene_layer->get_entity(collider2->get_entity_id());
				std::string collision_event_key = entity1->get_name() + "_and_" + entity2->get_name();

				/* Collision Detected */
				if ((box1left <= box2right && box1right >= box2left) &&
					(box1bottom <= box2top && box1top >= box2bottom) &&
					(box1back <= box2front && box1front >= box2back))
				{
					std::unordered_map<std::string, std::string> strings;
					std::unordered_map<std::string, std::shared_ptr<Component>> components;

					components.emplace("collider", collider1);
					components.emplace("other_collider", collider2);
					components.emplace("physics_body", scene_layer->get_component_by_type<PhysicsBody>(entity1->get_id()));
					components.emplace("other_physics_body", scene_layer->get_component_by_type<PhysicsBody>(entity2->get_id()));
					strings.emplace("collider_name", entity1->get_name());
					strings.emplace("other_collider_name", entity2->get_name());

					if (!EventBus::has_continuous_event(
						OMNIFIC_EVENT_ENTITY_IS_COLLIDING, 
						collision_event_key))
					{
						EventBus::publish_event(
							OMNIFIC_EVENT_ENTITY_IS_ON_COLLISION, 
							strings, 
							{}, 
							{}, 
							components, 
							collision_event_key);

						EventBus::publish_event(
							OMNIFIC_EVENT_ENTITY_IS_COLLIDING, 
							strings, 
							{}, 
							{}, 
							components, 
							collision_event_key,
							true);
					}
				}
				else if (EventBus::has_continuous_event(
					OMNIFIC_EVENT_ENTITY_IS_COLLIDING, 
					collision_event_key))
				{
					std::unordered_map<std::string, std::string> strings;

					strings.emplace("collider_name", entity1->get_name());
					strings.emplace("other_collider_name", entity2->get_name());
					EventBus::remove_continuous_event(OMNIFIC_EVENT_ENTITY_IS_COLLIDING, collision_event_key);
					EventBus::publish_event(OMNIFIC_EVENT_ENTITY_IS_OFF_COLLISION, strings);
				}
			}
		}
	}
}

void Omnific::PhysicsSystem::handle_collisions_and_displacements(std::shared_ptr<SceneLayer> scene_layer)
{
	std::vector<Event> collision_events = EventBus::query_events(OMNIFIC_EVENT_ENTITY_IS_COLLIDING);
	size_t collision_event_count = collision_events.size();

	/* Basic collision response on boxes */
	for (size_t i = 0; i < collision_event_count; i++)
	{
		Event& collision_event = collision_events.at(i);
		std::unordered_map<std::string, std::shared_ptr<Component>> components = collision_event.get_parameters().components;
		std::shared_ptr<PhysicsBody> physics_body = std::dynamic_pointer_cast<PhysicsBody>(components.at("physics_body"));

		if (physics_body != nullptr)
		{
			if (physics_body->get_scene_layer_id() == scene_layer->get_id())
			{
				std::shared_ptr<PhysicsBody> other_physics_body = std::dynamic_pointer_cast<PhysicsBody>(components.at("other_physics_body"));
				std::shared_ptr<Entity> entity = scene_layer->get_entity(physics_body->get_entity_id());
					
				switch (physics_body->collision_response_type)
				{
					case CollisionResponseType::RIGID: 
						if (other_physics_body != nullptr)
						{
							CollisionResponseType collision_response_type = other_physics_body->collision_response_type;
							if (collision_response_type == CollisionResponseType::KINEMATIC)
							{
								glm::vec3 reversed_linear_velocity = physics_body->linear_velocity * -1.0f;
								entity->get_transform()->translation += reversed_linear_velocity * 1.5f * (float)(Configuration::get_instance()->performance_settings.fixed_frame_time * (1.0 / MS_IN_S));
								physics_body->linear_velocity = (reversed_linear_velocity * physics_body->elasticity_ratio) + other_physics_body->linear_velocity;
							}
							else if (collision_response_type == CollisionResponseType::RIGID)
							{
								//Elastic collision response for now.
								std::shared_ptr<Entity> other_entity = scene_layer->get_entity(other_physics_body->get_entity_id());
								glm::vec3 v1 = physics_body->linear_velocity;
								glm::vec3 v2 = other_physics_body->linear_velocity;
								float m1 = physics_body->mass;
								float m2 = other_physics_body->mass;
								glm::vec3 n = entity->get_transform()->translation - other_entity->get_transform()->translation;
								physics_body->linear_velocity = v1 - (float)(((2.0 * m2)/(m1 + m2)) * ((glm::dot(v1-v2, n))/(pow(n.length(), 2.0)))) * n;
							}
							else if (collision_response_type == CollisionResponseType::FLUID)
							{
								std::shared_ptr<Collider> collider = scene_layer->get_component_by_type<Collider>(entity->get_id());
								float approximate_radius = (collider->box.aabb.max - collider->box.aabb.min).length();
								glm::vec3 deceleration = (float)(6.0 * M_PI * other_physics_body->viscosity * approximate_radius * (1.0 / physics_body->mass)) * physics_body->linear_velocity;
								physics_body->linear_velocity -= deceleration;
							}
						}
						break;
				}
			}
		}
	}

	/*Displacements*/
	const float seconds_per_fixed_update = Configuration::get_instance()->performance_settings.fixed_frame_time * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
	{
		scene_layer->get_entity(physics_body->get_entity_id())->get_transform()->translation += physics_body->linear_velocity * seconds_per_fixed_update;
	}
}