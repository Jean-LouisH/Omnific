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
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		SceneTree& sceneTree = it->second;
		this->updateTimers(sceneTree);
		this->gravitate(sceneTree);
		this->applyForces(sceneTree);
		this->decelerate(sceneTree);
		this->detectCollisions(sceneTree);
		this->handleCollisions(sceneTree);
		this->displace(sceneTree);
	}
}

void Omnific::PhysicsSystem::deinitialize()
{
	this->isInitialized = false;
}

void Omnific::PhysicsSystem::updateTimers(SceneTree& sceneTree)
{
	std::vector<std::shared_ptr<CountdownTimer>> countdownTimers = sceneTree.getComponentsByType<CountdownTimer>();

	for (size_t i = 0; i < countdownTimers.size(); i++)
	{
		countdownTimers.at(i)->update(this->secondsPerComputeUpdate);
	}
}

void Omnific::PhysicsSystem::displace(SceneTree& sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree.getComponentsByType<RigidBody>();
	std::vector<std::shared_ptr<CharacterBody>> characterBodies = sceneTree.getComponentsByType<CharacterBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		std::shared_ptr<Transform> transform = sceneTree.getEntityTransform(rigidBody->getEntityID());
		transform->translation += rigidBody->linearVelocity * this->secondsPerComputeUpdate;
	}

	for (int i = 0; i < characterBodies.size(); i++)
	{
		std::shared_ptr<CharacterBody> characterBody = characterBodies.at(i);
		std::shared_ptr<Transform> transform = sceneTree.getEntityTransform(characterBody->getEntityID());
		//ToDo: translating according to the snap and up vectors.
		transform->translation += characterBody->linearVelocity * this->secondsPerComputeUpdate;
	}
}

void Omnific::PhysicsSystem::gravitate(SceneTree& sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree.getComponentsByType<RigidBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		rigidBody->linearVelocity.y -= rigidBody->gravityScale * EARTH_GRAVITY * this->secondsPerComputeUpdate;
	}
}

void Omnific::PhysicsSystem::decelerate(SceneTree& sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree.getComponentsByType<RigidBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		rigidBody->linearVelocity.x *= pow(rigidBody->dragRatio.x, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.y *= pow(rigidBody->dragRatio.y, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.z *= pow(rigidBody->dragRatio.z, this->secondsPerComputeUpdate);
	}
}

void Omnific::PhysicsSystem::applyForces(SceneTree& sceneTree)
{
	std::vector<std::shared_ptr<ConstantForce>> constantForces = sceneTree.getComponentsByType<ConstantForce>();

	for (size_t i = 0; i < constantForces.size(); i++)
	{
		std::shared_ptr<ConstantForce> constantForce = constantForces.at(i);
		std::shared_ptr<Transform> transform = sceneTree.getEntityTransform(constantForce->getEntityID());

		//Todo: Apply force on entities
	}
}

void Omnific::PhysicsSystem::detectCollisions(SceneTree& sceneTree)
{
	/* Very basic collision detection on boxes for now. */
	std::vector<std::shared_ptr<Collider>> colliders = sceneTree.getComponentsByType<Collider>();
	size_t collidersCount = colliders.size();

	for (size_t i = 0; i < collidersCount; i++)
	{
		std::shared_ptr<Collider> collider1 = colliders.at(i);
		std::shared_ptr<Transform> transform1 = sceneTree.getEntityTransform(collider1->getEntityID());
		glm::vec3 translation1 = transform1->translation;
		AABB3D aabb1 = collider1->box.aabb;

		for (size_t j = 0; j < collidersCount; j++)
		{
			if (i != j)
			{
				std::shared_ptr<Collider> collider2 = colliders.at(j);
				std::shared_ptr<Transform> transform2 = sceneTree.getEntityTransform(collider2->getEntityID());
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
					Entity& entity1 = sceneTree.getEntity(collider1->getEntityID());
					Entity& entity2 = sceneTree.getEntity(collider2->getEntityID());
					bool hasRigidBody1 = entity1.componentIDs.count(RigidBody::TYPE_STRING) > 0;
					bool hasRigidBody2 = entity2.componentIDs.count(RigidBody::TYPE_STRING) > 0;
					std::unordered_map<std::string, float> eventNumbers;

					eventNumbers.emplace("first_entity_id", collider1->getEntityID());
					eventNumbers.emplace("second_entity_id", collider2->getEntityID());
					eventNumbers.emplace("attack_angle", 0.0); //ToDo: 3D attack angle

					if (hasRigidBody1)
					{
						std::shared_ptr<Component> rigidBodyComponent1 = sceneTree.getComponent(entity1.componentIDs.at(RigidBody::TYPE_STRING));
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
						std::shared_ptr<Component> rigidBodyComponent2 = sceneTree.getComponent(entity2.componentIDs.at(RigidBody::TYPE_STRING));
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

					sceneTree.getEventBus().publish(this->collisionEventString, eventNumbers);
				}
			}
		}
	}
}

void Omnific::PhysicsSystem::handleCollisions(SceneTree& sceneTree)
{
	std::vector<Event> collisionEvents = sceneTree.getEventBus().query(this->collisionEventString);
	size_t collisionEventCount = collisionEvents.size();

	/* Basic collision response on boxes */
	for (size_t i = 0; i < collisionEventCount; i++)
	{
		Event& collisionEvent = collisionEvents.at(i);
		std::unordered_map<std::string, float> floats = collisionEvent.getParameters().floats;
		Entity& entity = sceneTree.getEntity(floats.at("first_entity_id"));

		/* Collision response for RigidBodies and CharacterBodies */
		if (entity.componentIDs.count(RigidBody::TYPE_STRING))
		{
			std::shared_ptr<Component> rigidBodyComponent = sceneTree.getComponent(entity.componentIDs.at(RigidBody::TYPE_STRING));
			std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent);
			rigidBody->linearVelocity.x = floats.at("second_linear_velocity_x");
			rigidBody->linearVelocity.y = floats.at("second_linear_velocity_y");
			rigidBody->linearVelocity.z = floats.at("second_linear_velocity_z");
		}
		else if (entity.componentIDs.count(CharacterBody::TYPE_STRING))
		{
			std::shared_ptr<Component> characterBodyComponent = sceneTree.getComponent(entity.componentIDs.at(CharacterBody::TYPE_STRING));
			std::shared_ptr<CharacterBody> characterBody = std::dynamic_pointer_cast<CharacterBody>(characterBodyComponent);
			characterBody->linearVelocity.x = floats.at("second_linear_velocity_x");
			characterBody->linearVelocity.y = floats.at("second_linear_velocity_y");
			characterBody->linearVelocity.z = floats.at("second_linear_velocity_z");
		}
	}
}

void Omnific::PhysicsSystem::onComputeEnd(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree> sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		SceneTree& sceneTree = it->second;
		std::vector<std::shared_ptr<CharacterBody>> characterBodies = sceneTree.getComponentsByType<CharacterBody>();

		for (int i = 0; i < characterBodies.size(); i++)
		{
			characterBodies.at(i)->reload();
		}
	}
}