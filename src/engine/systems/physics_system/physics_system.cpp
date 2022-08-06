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
#include <utilities/constants.hpp>
#include <os/os.hpp>

Omnia::PhysicsSystem::~PhysicsSystem()
{
	this->deinitialize();
}

void Omnia::PhysicsSystem::setMsPerComputeUpdate(uint32_t msPerComputeUpdate)
{
	this->secondsPerComputeUpdate = msPerComputeUpdate * (1.0 / MS_IN_S);
}

void Omnia::PhysicsSystem::initialize()
{
	this->isInitialized = true;
}

void Omnia::PhysicsSystem::process(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		std::shared_ptr<SceneTree> sceneTree = it->second;
		this->updateTimers(sceneTree);
		this->gravitate(sceneTree);
		this->applyForces(sceneTree);
		this->decelerate(sceneTree);
		this->detectCollisions(sceneTree);
		this->handleCollisions(sceneTree);
		this->displace(sceneTree);
	}
}

void Omnia::PhysicsSystem::deinitialize()
{
	this->isInitialized = false;
}

void Omnia::PhysicsSystem::updateTimers(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<CountdownTimer>> countdownTimers = sceneTree->getComponentsByType<CountdownTimer>();

	for (size_t i = 0; i < countdownTimers.size(); i++)
	{
		countdownTimers.at(i)->update(this->secondsPerComputeUpdate);
	}
}

void Omnia::PhysicsSystem::displace(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree->getComponentsByType<RigidBody>();
	std::vector<std::shared_ptr<CharacterBody>> characterBodies = sceneTree->getComponentsByType<CharacterBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		this->displaceEntityTree(sceneTree, rigidBody->getEntityID(), rigidBody->linearVelocity * this->secondsPerComputeUpdate);
	}

	for (int i = 0; i < characterBodies.size(); i++)
	{
		std::shared_ptr<CharacterBody> characterBody = characterBodies.at(i);
		this->displaceEntityTree(sceneTree, characterBody->getEntityID(), characterBody->linearVelocity * this->secondsPerComputeUpdate);
	}
}

void Omnia::PhysicsSystem::gravitate(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree->getComponentsByType<RigidBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		rigidBody->linearVelocity.y -= rigidBody->gravityScale * EARTH_GRAVITY * this->secondsPerComputeUpdate;
	}
}

void Omnia::PhysicsSystem::decelerate(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies = sceneTree->getComponentsByType<RigidBody>();

	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> rigidBody = rigidBodies.at(i);
		rigidBody->linearVelocity.x *= pow(rigidBody->dragRatio.x, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.y *= pow(rigidBody->dragRatio.y, this->secondsPerComputeUpdate);
		rigidBody->linearVelocity.z *= pow(rigidBody->dragRatio.z, this->secondsPerComputeUpdate);
	}
}

void Omnia::PhysicsSystem::applyForces(std::shared_ptr<SceneTree> sceneTree)
{
	std::vector<std::shared_ptr<ConstantForce>> constantForces = sceneTree->getComponentsByType<ConstantForce>();

	for (size_t i = 0; i < constantForces.size(); i++)
	{
		std::shared_ptr<ConstantForce> constantForce = constantForces.at(i);
		std::shared_ptr<Transform> transform = sceneTree->getEntityTransform(constantForce->getEntityID());

		//Todo: Apply force on entities
	}
}

void Omnia::PhysicsSystem::detectCollisions(std::shared_ptr<SceneTree> sceneTree)
{
	/* Very basic collision detection on boxes for now. */
	std::vector<std::shared_ptr<Collider>> colliders = sceneTree->getComponentsByType<Collider>();
	size_t collidersCount = colliders.size();
	std::string collisionEventString;

	for (size_t i = 0; i < collidersCount; i++)
	{
		std::shared_ptr<Collider> collider1 = colliders.at(i);
		std::shared_ptr<Transform> transform1 = sceneTree->getEntityTransform(collider1->getEntityID());
		glm::vec3 translation1 = transform1->translation;
		glm::vec3 scale1 = transform1->scale;
		AABB3D aabb1 = collider1->box.aabb;

		for (size_t j = 0; j < collidersCount; j++)
		{
			if (i != j)
			{
				std::shared_ptr<Collider> collider2 = colliders.at(j);
				std::shared_ptr<Transform> transform2 = sceneTree->getEntityTransform(collider2->getEntityID());
				glm::vec3 translation2 = transform2->translation;
				glm::vec3 scale2 = transform2->scale;
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

				std::shared_ptr<Entity> entity1 = sceneTree->getEntity(collider1->getEntityID());
				std::shared_ptr<Entity> entity2 = sceneTree->getEntity(collider2->getEntityID());
				std::shared_ptr<CollisionRegistry> collisionRegistry = sceneTree->getCollisionRegistry();

				/* Collision Detected */
				if ((box1left <= box2right && box1right >= box2left) &&
					(box1bottom <= box2top && box1top >= box2bottom) &&
					(box1back <= box2front && box1front >= box2back))
				{
					std::shared_ptr<Collision> collision(new Collision());
					std::unordered_map<std::string, std::string> eventStrings;

					collision->colliderEntityID = collider1->getEntityID();
					collision->colliderName = entity1->getName();
					eventStrings.emplace("collider_name", entity1->getName());
					collision->otherColliderEntityID = collider2->getEntityID();
					collision->otherColliderName = entity2->getName();
					eventStrings.emplace("other_collider_name", entity2->getName());

					collision->hasRigidbody = entity1->componentIDs.count(RigidBody::TYPE_STRING) > 0;
					collision->hasOtherRigidbody = entity2->componentIDs.count(RigidBody::TYPE_STRING) > 0;

					if (collision->hasRigidbody)
					{
						std::shared_ptr<Component> rigidBodyComponent1 = sceneTree->getComponent(entity1->componentIDs.at(RigidBody::TYPE_STRING));
						std::shared_ptr<RigidBody> rigidBody1 = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent1);

						collision->elasticityRatio = rigidBody1->elasticityRatio;
						collision->mass = rigidBody1->mass;
						collision->linearVelocity = rigidBody1->linearVelocity;
						collision->rotation = transform1->rotation;
					}

					if (collision->hasOtherRigidbody)
					{
						std::shared_ptr<Component> rigidBodyComponent2 = sceneTree->getComponent(entity2->componentIDs.at(RigidBody::TYPE_STRING));
						std::shared_ptr<RigidBody> rigidBody2 = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent2);

						collision->otherElasticityRatio = rigidBody2->elasticityRatio;
						collision->otherMass = rigidBody2->mass;
						collision->otherLinearVelocity = rigidBody2->linearVelocity;
						collision->otherRotation = transform2->rotation;
					}
					else if (!collider2->isTrigger)
					{
						collision->otherElasticityRatio = 1.0;
						collision->otherMass = EARTH_MASS;
						collision->otherRotation = transform2->rotation;
					}

					if (!collisionRegistry->isColliding(entity1->getName(), entity2->getName()))
						sceneTree->getEventBus()->publish("entity_is_on_collision", eventStrings);

					collisionRegistry->addOrUpdate(collision);
				}
				else if (collisionRegistry->isColliding(entity1->getName(), entity2->getName()))
				{
					std::unordered_map<std::string, std::string> eventStrings;
					eventStrings.emplace("collider_name", entity1->getName());
					eventStrings.emplace("other_collider_name", entity2->getName());
					collisionRegistry->remove(entity1->getName(), entity2->getName());
					sceneTree->getEventBus()->publish("entity_is_off_collision", eventStrings);
				}
			}
		}
	}
}

void Omnia::PhysicsSystem::handleCollisions(std::shared_ptr<SceneTree> sceneTree)
{
	std::shared_ptr<EventBus> eventBus = sceneTree->getEventBus();
	std::vector<Event> collisionEvents = eventBus->query("entity_is_on_collision");
	size_t collisionEventCount = collisionEvents.size();

	/* Basic collision response on boxes */
	for (size_t i = 0; i < collisionEventCount; i++)
	{
		Event& collisionEvent = collisionEvents.at(i);
		std::unordered_map<std::string, double> numbers = collisionEvent.getParameters().numbers;

		if (!numbers.empty())
		{
			std::shared_ptr<Entity> entity = sceneTree->getEntity(numbers.at("first_entity_id"));

			/* Collision response for RigidBodies and CharacterBodies */
			if (entity->componentIDs.count(RigidBody::TYPE_STRING))
			{
				std::shared_ptr<Component> rigidBodyComponent = sceneTree->getComponent(entity->componentIDs.at(RigidBody::TYPE_STRING));
				std::shared_ptr<RigidBody> rigidBody = std::dynamic_pointer_cast<RigidBody>(rigidBodyComponent);
				rigidBody->linearVelocity.x = numbers.at("second_linear_velocity_x");
				rigidBody->linearVelocity.y = numbers.at("second_linear_velocity_y");
				rigidBody->linearVelocity.z = numbers.at("second_linear_velocity_z");
			}
			else if (entity->componentIDs.count(CharacterBody::TYPE_STRING))
			{
				std::shared_ptr<Component> characterBodyComponent = sceneTree->getComponent(entity->componentIDs.at(CharacterBody::TYPE_STRING));
				std::shared_ptr<CharacterBody> characterBody = std::dynamic_pointer_cast<CharacterBody>(characterBodyComponent);
				characterBody->linearVelocity.x = numbers.at("second_linear_velocity_x");
				characterBody->linearVelocity.y = numbers.at("second_linear_velocity_y");
				characterBody->linearVelocity.z = numbers.at("second_linear_velocity_z");
			}
		}
	}
}

void Omnia::PhysicsSystem::onComputeEnd(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>> sceneTrees = scene->getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		std::shared_ptr<SceneTree> sceneTree = it->second;
		std::vector<std::shared_ptr<CharacterBody>> characterBodies = sceneTree->getComponentsByType<CharacterBody>();

		for (int i = 0; i < characterBodies.size(); i++)
		{
			characterBodies.at(i)->reload();
		}
	}
}

void Omnia::PhysicsSystem::displaceEntityTree(std::shared_ptr<SceneTree> sceneTree, EntityID entityID, glm::vec3 value)
{
	std::shared_ptr<Transform> transform = sceneTree->getEntityTransform(entityID);
	std::shared_ptr<Entity> entity = sceneTree->getEntity(entityID);
	transform->translation += value;

	for (size_t i = 0; i < entity->childIDs.size(); i++)
		this->displaceEntityTree(sceneTree, entity->childIDs.at(i), value);
}