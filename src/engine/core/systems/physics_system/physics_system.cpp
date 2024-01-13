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
#include "core/scene/scene.hpp"
#include <core/utilities/constants.hpp>
#include <core/singletons/os/os.hpp>
#include <core/singletons/configuration.hpp>

#include <core/components/collider.hpp>
#include <core/components/physics_body.hpp>
#include <core/components/timer.hpp>
#include <core/components/transform.hpp>

#include <core/singletons/event_bus.hpp>


Omnia::PhysicsSystem::~PhysicsSystem()
{
	this->finalize();
}

void Omnia::PhysicsSystem::initialize()
{
	this->is_initialized = true;
	OS::get_logger().write("Initialized Physics System");
}

void Omnia::PhysicsSystem::on_compute(std::shared_ptr<Scene> scene)
{
	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::shared_ptr<SceneLayer> scene_layer = scene_layer_it.second;
		this->update_timers(scene_layer);
		this->gravitate(scene_layer);
		this->decelerate(scene_layer);
		this->detect_collisions(scene_layer);
		this->handle_collisions(scene_layer);
		this->displace(scene_layer);
	}
}

void Omnia::PhysicsSystem::finalize()
{
	this->is_initialized = false;
}

void Omnia::PhysicsSystem::update_timers(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update * (1.0 / MS_IN_S);

	for (std::shared_ptr<Timer> timer : scene_layer->get_components_by_type<Timer>())
		timer->update(seconds_per_compute_update);
}

void Omnia::PhysicsSystem::displace(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
	{
		std::shared_ptr<Transform> transform = scene_layer->get_component_by_type<Transform>(physics_body->get_entity_id());
		if (transform != nullptr)
			transform->translation += physics_body->linear_velocity * seconds_per_compute_update;
	}
}

void Omnia::PhysicsSystem::gravitate(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
		if (physics_body->is_rigid_body)
			physics_body->linear_velocity.y -= physics_body->gravity_scale * EARTH_GRAVITY * seconds_per_compute_update;
}

void Omnia::PhysicsSystem::decelerate(std::shared_ptr<SceneLayer> scene_layer)
{
	const float seconds_per_compute_update = Configuration::get_instance()->time_settings.ms_per_compute_update * (1.0 / MS_IN_S);

	for (std::shared_ptr<PhysicsBody> physics_body : scene_layer->get_components_by_type<PhysicsBody>())
	{
		if (physics_body->is_rigid_body)
		{
			physics_body->linear_velocity.x *= pow(physics_body->drag_ratio.x, seconds_per_compute_update);
			physics_body->linear_velocity.y *= pow(physics_body->drag_ratio.y, seconds_per_compute_update);
			physics_body->linear_velocity.z *= pow(physics_body->drag_ratio.z, seconds_per_compute_update);
		}
	}
}

void Omnia::PhysicsSystem::detect_collisions(std::shared_ptr<SceneLayer> scene_layer)
{
	/* Very basic collision detection on boxes for now. */
	std::vector<std::shared_ptr<Collider>> colliders = scene_layer->get_components_by_type<Collider>();
	size_t colliders_count = colliders.size();
	std::string collision_event_string;

	for (size_t i = 0; i < colliders_count; i++)
	{
		std::shared_ptr<Collider> collider1 = colliders.at(i);
		std::shared_ptr<Transform> transform1 = scene_layer->get_component_by_type<Transform>(collider1->get_entity_id());
		glm::vec3 translation1 = transform1->translation;
		glm::vec3 scale1 = transform1->scale;
		scale1 *= 0.5;
		AABB3D aabb1 = collider1->box.aabb;

		for (size_t j = 0; j < colliders_count; j++)
		{
			if (i != j)
			{
				std::shared_ptr<Collider> collider2 = colliders.at(j);
				std::shared_ptr<Transform> transform2 = scene_layer->get_component_by_type<Transform>(collider2->get_entity_id());
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
				std::shared_ptr<CollisionRegistry> collision_registry = scene_layer->get_collision_registry();

				/* Collision Detected */
				if ((box1left <= box2right && box1right >= box2left) &&
					(box1bottom <= box2top && box1top >= box2bottom) &&
					(box1back <= box2front && box1front >= box2back))
				{
					std::shared_ptr<Collision> collision(new Collision());
					std::unordered_map<std::string, std::string> event_strings;

					collision->collider_entity_id = collider1->get_entity_id();
					collision->collider_name = entity1->get_name();
					event_strings.emplace("collider_name", entity1->get_name());
					collision->other_collider_entity_id = collider2->get_entity_id();
					collision->other_collider_name = entity2->get_name();
					event_strings.emplace("other_collider_name", entity2->get_name());

					collision->is_trigger_only = collider1->is_trigger_only;

					if (!collision->is_trigger_only)
					{
						collision->has_physics_body = entity1->component_ids.count(PhysicsBody::TYPE_STRING) > 0;
						collision->has_other_physics_body = entity2->component_ids.count(PhysicsBody::TYPE_STRING) > 0;

						if (collision->has_physics_body)
						{
							std::shared_ptr<Component> physics_body_component1 = scene_layer->get_component_by_id(entity1->component_ids.at(PhysicsBody::TYPE_STRING));
							std::shared_ptr<PhysicsBody> physics_body1 = std::dynamic_pointer_cast<PhysicsBody>(physics_body_component1);

							collision->elasticity_ratio = physics_body1->elasticity_ratio;
							collision->mass = physics_body1->mass;
							collision->linear_velocity = physics_body1->linear_velocity;
							collision->rotation = transform1->rotation;
						}

						if (collision->has_other_physics_body)
						{
							std::shared_ptr<Component> physics_body_component2 = scene_layer->get_component_by_id(entity2->component_ids.at(PhysicsBody::TYPE_STRING));
							std::shared_ptr<PhysicsBody> physics_body2 = std::dynamic_pointer_cast<PhysicsBody>(physics_body_component2);

							collision->other_elasticity_ratio = physics_body2->elasticity_ratio;
							collision->other_mass = physics_body2->mass;
							collision->other_linear_velocity = physics_body2->linear_velocity;
							collision->other_rotation = transform2->rotation;
						}
						else if (!collider2->is_trigger_only)
						{
							collision->other_elasticity_ratio = 1.0;
							collision->other_mass = EARTH_MASS;
							collision->other_rotation = transform2->rotation;
						}
					}

					if (!collision_registry->is_colliding(entity1->get_name(), entity2->get_name()))
						EventBus::publish("entity_is_on_collision", event_strings);

					collision_registry->add_or_update(collision);
				}
				else if (collision_registry->is_colliding(entity1->get_name(), entity2->get_name()))
				{
					std::unordered_map<std::string, std::string> event_strings;
					event_strings.emplace("collider_name", entity1->get_name());
					event_strings.emplace("other_collider_name", entity2->get_name());
					collision_registry->remove(entity1->get_name(), entity2->get_name());
					EventBus::publish("entity_is_off_collision", event_strings);
				}
			}
		}
	}
}

void Omnia::PhysicsSystem::handle_collisions(std::shared_ptr<SceneLayer> scene_layer)
{
	//std::shared_ptr<EventBus> event_bus = scene_layer->get_event_bus();
	//std::vector<Event> collision_events = event_bus->query("entity_is_on_collision");
	//size_t collision_event_count = collision_events.size();

	///* Basic collision response on boxes */
	//for (size_t i = 0; i < collision_event_count; i++)
	//{
	//	Event& collision_event = collision_events.at(i);
	//	std::unordered_map<std::string, double> numbers = collision_event.get_parameters().numbers;

	//	if (!numbers.empty())
	//	{
	//		std::shared_ptr<Entity> entity = scene_layer->get_entity(numbers.at("first_entity_id"));

	//		/* Collision response for PhysicsBodies */
	//		if (entity->component_ids.count(PhysicsBody::TYPE_STRING))
	//		{
	//			std::shared_ptr<Component> physics_body_component = scene_layer->get_component_by_id(entity->component_ids.at(PhysicsBody::TYPE_STRING));
	//			std::shared_ptr<PhysicsBody> physics_body = std::dynamic_pointer_cast<PhysicsBody>(physics_body_component);
	//			physics_body->linear_velocity.x = numbers.at("second_linear_velocity_x");
	//			physics_body->linear_velocity.y = numbers.at("second_linear_velocity_y");
	//			physics_body->linear_velocity.z = numbers.at("second_linear_velocity_z");
	//		}
	//	}
	//}
}