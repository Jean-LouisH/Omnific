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
#include "application/scene/scene.hpp"
#include <utilities/constants.hpp>
#include <os/os.hpp>

Omnific::PhysicsSystem::~PhysicsSystem()
{
	this->deinitialize();
}

void Omnific::PhysicsSystem::setMsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	this->secondsPerComputeUpdate = msPerComputeUpdate * (1.0 / MS_IN_S);
}

void Omnific::PhysicsSystem::initialize()
{
	this->isInitialized = true;
}

void Omnific::PhysicsSystem::process(Scene& scene)
{
	this->updateTimers(scene);
	this->gravitate(scene);
	this->applyForces(scene);
	this->decelerate(scene);
	this->detectCollisions(scene);
	this->handleCollisions(scene);
	this->displace(scene);
}

void Omnific::PhysicsSystem::deinitialize()
{
	this->isInitialized = false;
}

void Omnific::PhysicsSystem::updateTimers(Scene& scene)
{
	ComponentIterables countdownTimerIterables = scene.getComponentIterables(CountdownTimer::TYPE_STRING);

	for (size_t i = 0; i < countdownTimerIterables.count; i++)
	{
		std::shared_ptr<CountdownTimer> countdownTimer = std::dynamic_pointer_cast<CountdownTimer>(
			countdownTimerIterables.components.at(countdownTimerIterables.indexCache.at(i)));

		countdownTimer->update(this->secondsPerComputeUpdate);
	}
}

void Omnific::PhysicsSystem::displace(Scene& scene)
{
	ComponentIterables rigidBodyIterables = scene.getComponentIterables(RigidBody::TYPE_STRING);
	ComponentIterables characterBodyIterables = scene.getComponentIterables(CharacterBody::TYPE_STRING);

	for (size_t i = 0; i < rigidBodyIterables.count; i++)
	{
		std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(
			rigidBodyIterables.components.at(rigidBodyIterables.indexCache.at(i)));
		std::shared_ptr<Transform> transform = scene.getEntityTransform(rigidBody->getEntityID());

		transform->translation += rigidBody->linearVelocity * this->secondsPerComputeUpdate;
	}

	for (int i = 0; i < characterBodyIterables.count; i++)
	{
		std::shared_ptr<CharacterBody> characterBody = std::dynamic_pointer_cast<CharacterBody>(
			characterBodyIterables.components.at(characterBodyIterables.indexCache.at(i)));
		std::shared_ptr<Transform> transform = scene.getEntityTransform(characterBody->getEntityID());

		//ToDo: translating according to the snap and up vectors.
		transform->translation += characterBody->linearVelocity * this->secondsPerComputeUpdate;
	}
}

void Omnific::PhysicsSystem::gravitate(Scene& scene)
{
	ComponentIterables rigidBodyIterables = scene.getComponentIterables(RigidBody::TYPE_STRING);

	for (size_t i = 0; i < rigidBodyIterables.count; i++)
	{
		std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(
			rigidBodyIterables.components.at(rigidBodyIterables.indexCache.at(i)));

		rigidBody->linearVelocity.y -= rigidBody->gravityScale * EARTH_GRAVITY * this->secondsPerComputeUpdate;
	}
}

void Omnific::PhysicsSystem::decelerate(Scene& scene)
{
	ComponentIterables rigidBodyIterables = scene.getComponentIterables(RigidBody::TYPE_STRING);

	for (size_t i = 0; i < rigidBodyIterables.count; i++)
	{
		std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(
			rigidBodyIterables.components.at(rigidBodyIterables.indexCache.at(i)));

		rigidBody->linearVelocity.x *= pow(rigidBody->dragRatio.x, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.y *= pow(rigidBody->dragRatio.y, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.z *= pow(rigidBody->dragRatio.z, this->secondsPerComputeUpdate);
	}
}

void Omnific::PhysicsSystem::applyForces(Scene& scene)
{
	ComponentIterables constantForceIterables = scene.getComponentIterables(ConstantForce::TYPE_STRING);

	for (size_t i = 0; i < constantForceIterables.count; i++)
	{
		std::shared_ptr<ConstantForce> constantForce = std::dynamic_pointer_cast<ConstantForce>(
			constantForceIterables.components.at(constantForceIterables.indexCache.at(i)));
		std::shared_ptr<Transform> transform = scene.getEntityTransform(constantForce->getEntityID());

		//Todo: Apply force on entities
	}
}

void Omnific::PhysicsSystem::detectCollisions(Scene& scene)
{
	/* Very basic collision detection on boxes for now. */
	ComponentIterables colliderIterables = scene.getComponentIterables(Collider::TYPE_STRING);

	for (size_t i = 0; i < colliderIterables.count; i++)
	{
		std::shared_ptr<Collider> collider1 = std::dynamic_pointer_cast<Collider>(
			colliderIterables.components.at(colliderIterables.indexCache.at(i)));
		std::shared_ptr<Transform> transform1 = scene.getEntityTransform(collider1->getEntityID());
		glm::vec3 translation1 = transform1->translation;
		AABB3D aabb1 = collider1->box.aabb;

		for (size_t j = 0; j < colliderIterables.count; j++)
		{
			if (i != j)
			{
				std::shared_ptr<Collider> collider2 = std::dynamic_pointer_cast<Collider>(
					colliderIterables.components.at(colliderIterables.indexCache.at(j)));
				std::shared_ptr<Transform> transform2 = scene.getEntityTransform(collider2->getEntityID());
				glm::vec3 translation2 = transform2->translation;
				AABB3D aabb2 = collider2->box.aabb;

				float box1left = translation1.x + aabb1.min.x;
				float box1right = translation1.x + aabb1.max.x;
				float box1top = translation1.y + aabb1.max.y;
				float box1bottom = translation1.y + aabb1.min.y;
				float box1front = translation1.z + aabb1.max.z;
				float box1back = translation1.z + aabb1.min.z;

				float box2left = translation2.x + aabb2.min.x;
				float box2right = translation2.x + aabb2.max.x;
				float box2top = translation2.y + aabb2.max.y;
				float box2bottom = translation2.y + aabb2.min.y;
				float box2front = translation2.z + aabb2.max.z;
				float box2back = translation2.z + aabb2.min.z;

				/* Collision Detected */
				if ((box1left <= box2right && box1right >= box2left) &&
					(box1bottom <= box2top && box1top >= box2bottom) &&
					(box1back <= box2front && box1front >= box2back))
				{
					Entity& entity1 = scene.getEntity(collider1->getEntityID());
					Entity& entity2 = scene.getEntity(collider2->getEntityID());
					bool hasRigidBody1 = entity1.components.count(RigidBody::TYPE_STRING) > 0;
					bool hasRigidBody2 = entity2.components.count(RigidBody::TYPE_STRING) > 0;
					std::unordered_map<std::string, float> eventNumbers;

					eventNumbers.emplace("first_entity_id", collider1->getEntityID());
					eventNumbers.emplace("second_entity_id", collider2->getEntityID());
					eventNumbers.emplace("attack_angle", 0.0); //ToDo: 3D attack angle

					if (hasRigidBody1)
					{
						std::shared_ptr<Component> rigidBodyComponent1 = scene.getComponent(entity1.components.at(RigidBody::TYPE_STRING));
						std::shared_ptr<RigidBody> rigidBody1 = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent1);

						eventNumbers.emplace("first_elasticity_ratio", rigidBody1->elasticityRatio);
						eventNumbers.emplace("first_mass", rigidBody1->mass);
						eventNumbers.emplace("first_linear_velocity_x", rigidBody1->linearVelocity.x);
						eventNumbers.emplace("first_linear_velocity_y", rigidBody1->linearVelocity.y);
						eventNumbers.emplace("first_linear_velocity_z", rigidBody1->linearVelocity.z);
						eventNumbers.emplace("first_rotation_x", transform1->rotation.x);
						eventNumbers.emplace("first_rotation_y", transform1->rotation.y);
						eventNumbers.emplace("first_rotation_z", transform1->rotation.z);
					}

					if (hasRigidBody2)
					{
						std::shared_ptr<Component> rigidBodyComponent2 = scene.getComponent(entity2.components.at(RigidBody::TYPE_STRING));
						std::shared_ptr<RigidBody> rigidBody2 = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent2);

						eventNumbers.emplace("second_elasticity_ratio", rigidBody2->elasticityRatio);
						eventNumbers.emplace("second_mass", rigidBody2->mass);
						eventNumbers.emplace("second_linear_velocity_x", rigidBody2->linearVelocity.x);
						eventNumbers.emplace("second_linear_velocity_y", rigidBody2->linearVelocity.y);
						eventNumbers.emplace("second_linear_velocity_z", rigidBody2->linearVelocity.z);
						eventNumbers.emplace("second_rotation_x", transform2->rotation.x);
						eventNumbers.emplace("second_rotation_y", transform2->rotation.y);
						eventNumbers.emplace("second_rotation_z", transform2->rotation.z);
					}
					else if (!collider2->isTrigger)
					{
						eventNumbers.emplace("second_elasticity_ratio", 1.0);
						eventNumbers.emplace("second_mass", EARTH_MASS);
						eventNumbers.emplace("second_linear_velocity_x", 0.0);
						eventNumbers.emplace("second_linear_velocity_y", 0.0);
						eventNumbers.emplace("second_linear_velocity_z", 0.0);
						eventNumbers.emplace("second_rotation_x", transform2->rotation.x);
						eventNumbers.emplace("second_rotation_y", transform2->rotation.y);
						eventNumbers.emplace("second_rotation_z", transform2->rotation.z);
					}

					scene.getEventBus().publish(this->collisionEventString, eventNumbers);
				}
			}
		}
	}
}

void Omnific::PhysicsSystem::handleCollisions(Scene& scene)
{
	std::vector<Event> collisionEvents = scene.getEventBus().query(this->collisionEventString);
	size_t collisionEventCount = collisionEvents.size();

	/* Basic collision response on boxes */
	for (size_t i = 0; i < collisionEventCount; i++)
	{
		Event& collisionEvent = collisionEvents.at(i);
		std::unordered_map<std::string, float> floats = collisionEvent.getParameters().floats;
		Entity& entity = scene.getEntity(floats.at("first_entity_id"));

		/* Collision response for RigidBodies and CharacterBodies */
		if (entity.components.count(RigidBody::TYPE_STRING))
		{
			std::shared_ptr<Component> rigidBodyComponent = scene.getComponent(entity.components.at(RigidBody::TYPE_STRING));
			std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent);
			rigidBody->linearVelocity.x = floats.at("second_linear_velocity_x");
			rigidBody->linearVelocity.y = floats.at("second_linear_velocity_y");
			rigidBody->linearVelocity.z = floats.at("second_linear_velocity_z");
		}
		else if (entity.components.count(CharacterBody::TYPE_STRING))
		{
			std::shared_ptr<Component> characterBodyComponent = scene.getComponent(entity.components.at(CharacterBody::TYPE_STRING));
			std::shared_ptr<CharacterBody> characterBody = std::dynamic_pointer_cast<CharacterBody>(characterBodyComponent);
			characterBody->linearVelocity.x = floats.at("second_linear_velocity_x");
			characterBody->linearVelocity.y = floats.at("second_linear_velocity_y");
			characterBody->linearVelocity.z = floats.at("second_linear_velocity_z");
		}
	}
}

void Omnific::PhysicsSystem::onComputeEnd(Scene& scene)
{
	ComponentIterables characterBodyIterables = scene.getComponentIterables(CharacterBody::TYPE_STRING);

	for (int i = 0; i < characterBodyIterables.count; i++)
	{
		std::shared_ptr<CharacterBody> characterBody = std::dynamic_pointer_cast<CharacterBody>(
			characterBodyIterables.components.at(characterBodyIterables.indexCache.at(i)));

		characterBody->reload();
	}
}