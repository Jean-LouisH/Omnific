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

#include "foundations/aliases.hpp"
#include "foundations/constants.hpp"
#include "foundations/aabb_2d.hpp"
#include "foundations/aabb_3d.hpp"
#include "scene/components/component.hpp"
#include <set>


namespace Omnific
{
	enum class CollisionShapeType
	{
		BOX,
		SPHERE,
		CAPSULE,
		MESH
	};

	class OMNIFIC_ENGINE_API Collider : public Component
	{
		friend class PhysicsSystem;

	public:
		std::set<EntityID> entering_entity_ids;
		std::set<std::string> entering_entity_tags;
		CollisionShapeType collision_shape_type;

		struct Box
		{
			enum CollidingSide
			{
				COLLIDING_SIDE_LEFT = 1 << 0,
				COLLIDING_SIDE_RIGHT = 1 << 1,
				COLLIDING_SIDE_TOP = 1 << 2,
				COLLIDING_SIDE_BOTTOM = 1 << 3,
				COLLIDING_SIDE_FRONT = 1 << 4,
				COLLIDING_SIDE_BACK = 1 << 5
			};

			uint8_t CollidingSides = 0;
			AABB3D aabb;
		}box;

		Collider()
		{
			this->type = TYPE_STRING;
		};

		static constexpr const char* TYPE_STRING = "Collider";

		virtual Registerable* instance() override
		{
			Collider* clone = new Collider(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		void set_dimensions(float width, float height, float depth);
		void set_collision_shape_type(CollisionShapeType collision_shape_type);
		bool is_collision_shape_type(CollisionShapeType collision_shape_type);
		bool has_entity_entered(EntityID entity_id);
		bool has_entity_with_tag_entered(std::string entity_tag);
	private:
		bool is_colliding = false;
	};
}