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


void Omnia::CollisionRegistry::add_or_update(std::shared_ptr<Collision> collision)
{
	std::string collision_entity_name = collision->collider_name;
	std::string other_collision_entity_name = collision->other_collider_name;

	if (!this->collisions.count(collision_entity_name))
	{
		std::unordered_map<OtherColliderName, std::shared_ptr<Collision>> collision_entry;
		collision_entry.emplace(other_collision_entity_name, collision);
		this->collisions.emplace(collision_entity_name, collision_entry);
	}
	else
	{
		this->collisions.at(collision_entity_name).emplace(other_collision_entity_name, collision);
	}
}

void Omnia::CollisionRegistry::remove(std::string collider_name, std::string other_collider_name)
{
	if (this->collisions.count(collider_name))
		if (this->collisions.at(collider_name).count(other_collider_name))
			this->collisions.at(collider_name).erase(other_collider_name);
}

std::shared_ptr<Omnia::Collision> Omnia::CollisionRegistry::query(std::string collider_name, std::string other_collider_name)
{
	std::shared_ptr<Collision> collision;

	if (this->collisions.count(collider_name))
	{
		if (this->collisions.at(collider_name).count(other_collider_name))
		{
			collision = this->collisions.at(collider_name).at(other_collider_name);
		}
	}
	else if (this->collisions.count(other_collider_name))
	{
		if (this->collisions.at(other_collider_name).count(collider_name))
		{
			collision = this->collisions.at(other_collider_name).at(collider_name);
		}
	}

	return collision;
}

std::unordered_map<std::string, std::shared_ptr<Omnia::Collision>> Omnia::CollisionRegistry::query_all(std::string collider_name)
{
	std::unordered_map<std::string, std::shared_ptr<Collision>> collision_entries;

	if (this->collisions.count(collider_name))
		collision_entries = this->collisions.at(collider_name);

	return collision_entries;
}

bool Omnia::CollisionRegistry::is_colliding(std::string collider_name, std::string other_collider_name)
{
	return (this->query(collider_name, other_collider_name) != nullptr);
}

uint64_t Omnia::CollisionRegistry::get_collision_count(std::string collider_name)
{
	return (uint64_t)(this->query_all(collider_name).size());
}