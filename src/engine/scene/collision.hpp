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

#include <foundations/aliases.hpp>
#include <glm.hpp>
#include <string>
#include <engine_api.hpp>


namespace Omnific
{
	typedef struct OMNIFIC_ENGINE_API Collision
	{
		EntityID collider_entity_id = 0;
		std::string collider_name;
		EntityID other_collider_entity_id = 0;
		std::string other_collider_name;
		glm::vec3 attack_angle;
		bool has_physics_body = false;
		bool has_other_physics_body = false;
		bool is_trigger_only = false;
		double elasticity_ratio = 0.0;
		double mass = 0.0;
		glm::vec3 linear_velocity;
		glm::vec3 rotation;
		double other_elasticity_ratio = 0.0;
		double other_mass = 0.0;
		glm::vec3 other_linear_velocity;
		glm::vec3 other_rotation;
	};
}