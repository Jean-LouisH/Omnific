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

#include "physics_body.hpp"

void Omnific::PhysicsBody::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "default")
		{

		}
		else if (it3->first.as<std::string>() == "is_rigidbody")
		{
			this->has_gravity = it3->second.as<bool>();
		}
		else if (it3->first.as<std::string>() == "drag_ratio")
		{
			this->drag_ratio[0] = it3->second[0].as<float>();
			this->drag_ratio[1] = it3->second[1].as<float>();
			this->drag_ratio[2] = it3->second[2].as<float>();
		}
		else if (it3->first.as<std::string>() == "collision_response_type")
		{
			if (it3->second.as<std::string>() == "intangible")
			{
				this->collision_response_type = CollisionResponseType::INTANGIBLE;
			}
			if (it3->second.as<std::string>() == "fluid")
			{
				this->collision_response_type = CollisionResponseType::FLUID;
			}
			if (it3->second.as<std::string>() == "soft")
			{
				this->collision_response_type = CollisionResponseType::SOFT;
			}
			else if (it3->second.as<std::string>() == "rigid")
			{
				this->collision_response_type = CollisionResponseType::RIGID;
			}
			else if (it3->second.as<std::string>() == "kinematic")
			{
				this->collision_response_type = CollisionResponseType::KINEMATIC;
			}
		}
	}
}

void Omnific::PhysicsBody::accelerate(glm::vec3 acceleration, float maximum_speed)
{
	this->linear_velocity += acceleration;
	float absolute_maximum_speed = abs(maximum_speed);
	float magnitude = sqrt(
		this->linear_velocity.x * this->linear_velocity.x +
		this->linear_velocity.y * this->linear_velocity.y +
		this->linear_velocity.z * this->linear_velocity.z);

	if (magnitude > absolute_maximum_speed)
	{
		this->linear_velocity = (absolute_maximum_speed * (abs(magnitude) / magnitude)) * 
								(this->linear_velocity / magnitude);
	}
}

void Omnific::PhysicsBody::accelerate(glm::vec3 direction, float acceleration, float maximum_speed)
{
	this->accelerate(direction * acceleration, maximum_speed);
}

void Omnific::PhysicsBody::accelerate_x(float acceleration, float maximum_speed)
{
	this->accelerate(glm::vec3(1.0, 0.0, 0.0), acceleration, maximum_speed);
}

void Omnific::PhysicsBody::accelerate_y(float acceleration, float maximum_speed)
{
	this->accelerate(glm::vec3(0.0, 1.0, 0.0), acceleration, maximum_speed);
}

void Omnific::PhysicsBody::accelerate_z(float acceleration, float maximum_speed)
{
	this->accelerate(glm::vec3(0.0, 0.0, 1.0), acceleration, maximum_speed);
}

void Omnific::PhysicsBody::decelerate(float deceleration)
{
	this->linear_velocity /= deceleration;
}

void Omnific::PhysicsBody::decelerate_x(float deceleration)
{
	this->linear_velocity.x /= deceleration;
}

void Omnific::PhysicsBody::decelerate_y(float deceleration)
{
	this->linear_velocity.y /= deceleration;
}

void Omnific::PhysicsBody::decelerate_z(float deceleration)
{
	this->linear_velocity.z /= deceleration;
}

void Omnific::PhysicsBody::move(glm::vec3 linear_velocity, glm::vec3 snap_direction, glm::vec3 up_direction)
{
	this->linear_velocity = linear_velocity;
	this->snap_direction = snap_direction;
	this->up_direction = up_direction;
}

void Omnific::PhysicsBody::reload()
{
	this->linear_velocity = glm::vec3(0.0, 0.0, 0.0);
	this->snap_direction = glm::vec3(0.0, 0.0, 0.0);
	this->up_direction = glm::vec3(0.0, 1.0, 0.0);
}