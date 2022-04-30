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

namespace Omnia
{
	class CollisionRegistry
	{
		using ColliderName = std::string;
		using OtherColliderName = std::string;

	public:
		CollisionRegistry()
		{
			collisions.reserve(32);
		}
		void addOrUpdate(std::shared_ptr<Collision> collision);
		void remove(std::string colliderName, std::string otherColliderName);
		std::shared_ptr<Collision> query(std::string colliderName, std::string otherColliderName);
		std::unordered_map<std::string, std::shared_ptr<Collision>> queryAll(std::string colliderName);
		bool isColliding(std::string colliderName, std::string otherColliderName);
		uint64_t getCollisionCount(std::string colliderName);
	private:
		std::unordered_map<ColliderName, std::unordered_map<OtherColliderName, std::shared_ptr<Collision>>> collisions;
	};
}