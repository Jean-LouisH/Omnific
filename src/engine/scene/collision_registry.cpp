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

#include "collision_registry.hpp"


void Omnia::CollisionRegistry::addOrUpdate(std::shared_ptr<Collision> collision)
{
	std::string collisionEntityName = collision->colliderName;
	std::string otherCollisionEntityName = collision->otherColliderName;

	if (!this->collisions.count(collisionEntityName))
	{
		std::unordered_map<OtherColliderName, std::shared_ptr<Collision>> collisionEntry;
		collisionEntry.emplace(otherCollisionEntityName, collision);
		this->collisions.emplace(collisionEntityName, collisionEntry);
	}
	else
	{
		this->collisions.at(collisionEntityName).emplace(otherCollisionEntityName, collision);
	}
}

void Omnia::CollisionRegistry::remove(std::string colliderName, std::string otherColliderName)
{
	if (this->collisions.count(colliderName))
		if (this->collisions.at(colliderName).count(otherColliderName))
			this->collisions.at(colliderName).erase(otherColliderName);
}

std::shared_ptr<Omnia::Collision> Omnia::CollisionRegistry::query(std::string colliderName, std::string otherColliderName)
{
	std::shared_ptr<Collision> collision;

	if (this->collisions.count(colliderName))
	{
		if (this->collisions.at(colliderName).count(otherColliderName))
		{
			collision = this->collisions.at(colliderName).at(otherColliderName);
		}
	}
	else if (this->collisions.count(otherColliderName))
	{
		if (this->collisions.at(otherColliderName).count(colliderName))
		{
			collision = this->collisions.at(otherColliderName).at(colliderName);
		}
	}

	return collision;
}

std::unordered_map<std::string, std::shared_ptr<Omnia::Collision>> Omnia::CollisionRegistry::queryAll(std::string colliderName)
{
	std::unordered_map<std::string, std::shared_ptr<Collision>> collisionEntries;

	if (this->collisions.count(colliderName))
		collisionEntries = this->collisions.at(colliderName);

	return collisionEntries;
}

bool Omnia::CollisionRegistry::isColliding(std::string colliderName, std::string otherColliderName)
{
	return (this->query(colliderName, otherColliderName) != nullptr);
}

uint64_t Omnia::CollisionRegistry::getCollisionCount(std::string colliderName)
{
	return (uint64_t)(this->queryAll(colliderName).size());
}