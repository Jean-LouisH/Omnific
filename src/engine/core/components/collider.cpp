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

#include "collider.hpp"

void Omnia::Collider::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "default")
		{

		}
		else if (it3->first.as<std::string>() == "dimensions")
		{
			this->setDimensions(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>());
		}
		else if (it3->first.as<std::string>() == "is_trigger_only")
		{
			this->isTriggerOnly = it3->second.as<bool>();
		}
	}
}

void Omnia::Collider::setDimensions(float width, float height, float depth)
{
	this->box.aabb.min = {-(width / 2.0), -(height / 2.0), -(depth / 2.0) };
	this->box.aabb.max = { (width / 2.0), (height / 2.0), (depth / 2.0) };
}

void Omnia::Collider::setColliderType(ColliderType colliderType)
{
	this->colliderType = colliderType;
}

bool Omnia::Collider::isColliderType(ColliderType colliderType)
{
	return this->colliderType == colliderType;
}

bool Omnia::Collider::hasEntityEntered(EntityID entityID)
{
	return this->enteringEntityIDs.count(entityID);
}

bool Omnia::Collider::hasEntityWithTagEntered(std::string entityTag)
{
	return this->enteringEntityTags.count(entityTag);
}