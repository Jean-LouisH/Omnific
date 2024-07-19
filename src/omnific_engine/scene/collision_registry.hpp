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

#pragma once

#include "collision.hpp"
#include <unordered_map>
#include <memory>
#include <stdint.h>
#include <engine_api.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API CollisionRegistry
	{
		using ColliderName = std::string;
		using OtherColliderName = std::string;

	public:
		CollisionRegistry()
		{
			collisions.reserve(32);
		}
		void add_or_update(std::shared_ptr<Collision> collision);
		void remove(std::string collider_name, std::string other_collider_name);
		std::shared_ptr<Collision> query(std::string collider_name, std::string other_collider_name);
		std::unordered_map<std::string, std::shared_ptr<Collision>> query_all(std::string collider_name);
		bool is_colliding(std::string collider_name, std::string other_collider_name);
		uint64_t get_collision_count(std::string collider_name);
	private:
		std::unordered_map<ColliderName, std::unordered_map<OtherColliderName, std::shared_ptr<Collision>>> collisions;
	};
}