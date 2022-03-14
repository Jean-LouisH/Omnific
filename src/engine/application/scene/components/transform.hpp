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


#include <glm/glm.hpp>
#include "application/scene/component.hpp"

namespace Omnia
{
	class Transform : public Component
	{
	public:
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scale;

		Transform() :
			rotation(0.0)
		{
			this->translation = glm::vec3(0.0, 0.0, 0.0);
			this->rotation = glm::vec3(0.0, 0.0, 0.0);
			this->scale = glm::vec3(1.0, 1.0, 1.0);
			this->type = TYPE_STRING;
		}

		static constexpr const char* TYPE_STRING = "Transform";

		void globallyTranslateX(float offset);
		void globallyTranslateY(float offset);
		void globallyTranslateZ(float offset);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		glm::mat4 getGlobalTransformMatrix();
	private:
	};
}
