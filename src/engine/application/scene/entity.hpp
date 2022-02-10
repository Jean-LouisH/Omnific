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

#include <application/scene/assets/shader.hpp>
#include "component.hpp"
#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <application/scene/uid_generator.hpp>

namespace Omnific
{
	typedef struct Entity
	{
		enum class SpatialDimension
		{
			_2D,
			_3D
		};

		EntityID id = DUMMY_ENTITY;
		EntityID parentID = DUMMY_ENTITY;
		SpatialDimension spatialDimension = SpatialDimension::_2D;
		std::vector<EntityID> childIDs;
		std::string name;
		std::vector<std::string> tags;
		std::unordered_map<std::string, ComponentID> components;

		std::vector<std::string> scripts;
		std::vector<Shader> shaders;

		Entity()
		{
			this->id = UIDGenerator::getNewUID();
			this->name = "Entity (ID:" + std::to_string(this->id) + ")";
		}
	};
}