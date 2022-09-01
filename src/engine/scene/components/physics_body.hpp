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

#include "utilities/aliases.hpp"
#include <glm/glm.hpp>
#include "utilities/constants.hpp"
#include "scene/components/component.hpp"

namespace Omnia
{
	class OMNIA_ENGINE_API PhysicsBody : public Component
	{
	public:
		PhysicsBody()
		{
			this->type = TYPE_STRING;
			this->dragRatio = glm::vec3(0.0001, 1.0, 0.0001);
			this->reload();
		};
		static constexpr const char* TYPE_STRING = "PhysicsBody";

		bool isRigidBody = false;

		float mass = 1.0;
		float frictionRatio = 1.0;
		float angularDragRatio = 1.0;
		float angularVelocity = 0.0;
		float gravityScale = 1.0;
		float elasticityRatio = 0.01;
		glm::vec3 dragRatio;
		glm::vec3 linearVelocity;
		glm::vec3 snapDirection;
		glm::vec3 upDirection;

		virtual Registerable* copy() override
		{
			return new PhysicsBody(*this);
		}
		void move(glm::vec3 linearVelocity, glm::vec3 snapDirection, glm::vec3 upDirection);
		void reload();
	private:
	};
}