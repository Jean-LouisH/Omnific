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

void Omnia::PhysicsBody::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "default")
		{

		}
		else if (it3->first.as<std::string>() == "is_rigidbody")
		{
			this->isRigidBody = it3->second.as<bool>();
		}
		else if (it3->first.as<std::string>() == "drag_ratio")
		{
			this->dragRatio[0] = it3->second[0].as<float>();
			this->dragRatio[1] = it3->second[1].as<float>();
			this->dragRatio[2] = it3->second[2].as<float>();
		}
	}
}

void Omnia::PhysicsBody::accelerate(glm::vec3 acceleration, float maximumSpeed)
{
	this->linearVelocity += acceleration;
	float absoluteMaximumSpeed = abs(maximumSpeed);
	float magnitude = sqrt(
		this->linearVelocity.x * this->linearVelocity.x +
		this->linearVelocity.y * this->linearVelocity.y +
		this->linearVelocity.z * this->linearVelocity.z);

	if (magnitude > absoluteMaximumSpeed)
	{
		this->linearVelocity = (absoluteMaximumSpeed * (abs(magnitude) / magnitude)) * 
								(this->linearVelocity / magnitude);
	}
}

void Omnia::PhysicsBody::accelerate(glm::vec3 direction, float acceleration, float maximumSpeed)
{
	this->accelerate(direction * acceleration, maximumSpeed);
}

void Omnia::PhysicsBody::accelerateX(float acceleration, float maximumSpeed)
{
	this->accelerate(glm::vec3(1.0, 0.0, 0.0), acceleration, maximumSpeed);
}

void Omnia::PhysicsBody::accelerateY(float acceleration, float maximumSpeed)
{
	this->accelerate(glm::vec3(0.0, 1.0, 0.0), acceleration, maximumSpeed);
}

void Omnia::PhysicsBody::accelerateZ(float acceleration, float maximumSpeed)
{
	this->accelerate(glm::vec3(0.0, 0.0, 1.0), acceleration, maximumSpeed);
}

void Omnia::PhysicsBody::decelerate(float deceleration)
{
	this->linearVelocity /= deceleration;
}

void Omnia::PhysicsBody::decelerateX(float deceleration)
{
	this->linearVelocity.x /= deceleration;
}

void Omnia::PhysicsBody::decelerateY(float deceleration)
{
	this->linearVelocity.y /= deceleration;
}

void Omnia::PhysicsBody::decelerateZ(float deceleration)
{
	this->linearVelocity.z /= deceleration;
}

void Omnia::PhysicsBody::move(glm::vec3 linearVelocity, glm::vec3 snapDirection, glm::vec3 upDirection)
{
	this->linearVelocity = linearVelocity;
	this->snapDirection = snapDirection;
	this->upDirection = upDirection;
}

void Omnia::PhysicsBody::reload()
{
	this->linearVelocity = glm::vec3(0.0, 0.0, 0.0);
	this->snapDirection = glm::vec3(0.0, 0.0, 0.0);
	this->upDirection = glm::vec3(0.0, 1.0, 0.0);
}