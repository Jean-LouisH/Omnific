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

#include <asset_pipeline/assets/shader.hpp>
#include "component.hpp"
#include "utilities/aliases.hpp"
#include "utilities/constants.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <uid_generator.hpp>
#include <memory>
#include <omnia_engine_api.hpp>

namespace Omnia
{
	/* An object that exists within the Scene with its own
	   identity, components and  hierarchy with other Entities. */
	class OMNIA_ENGINE_API Entity
	{
		friend class SceneTree;
	public:
		EntityID parentID = 0;
		std::vector<EntityID> childIDs;
		std::vector<std::string> tags;
		std::unordered_map<std::string, ComponentID> componentIDs;

		Entity()
		{
			this->id = UIDGenerator::getNewUID();
			this->name = "Entity (ID:" + std::to_string(this->id) + ")";
		}

		/*Sets name publicly only when it is not attached to a SceneTree.*/
		void setName(std::string name);
		std::string getName();
		EntityID getID();
	private:
		std::string name;
		EntityID id = 0;

		bool isAttachedToSceneTree = false;
	};
}