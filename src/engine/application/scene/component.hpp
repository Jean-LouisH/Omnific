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


#include <utilities/aliases.hpp>
#include <utilities/constants.hpp>
#include <application/scene/assets/image.hpp>
#include <string>
#include <unordered_map>
#include "component_property_pool.hpp"
#include <memory>

namespace Omnific
{
	/* Base class for objects that store and manipulate 
	   data relevant to individual Entities in a Scene.
	   These are attached to Entities when added to a Scene.*/
    class Component
    {
	public:
		Component();

		void setEntityID(EntityID entityID);
		void setComponentPropertyPool(std::shared_ptr<ComponentPropertyPool> componentPropertyPool);

		ComponentID getID();
		EntityID getEntityID();
		bool isAttachedToEntity();
		std::string getType() const;
		bool isType(std::string typeString);
		virtual bool isRenderable();
	protected:
		std::string type;

		void addProperty(std::string propertyName, float propertyValue);
		void updateProperty(std::string propertyName, float propertyValue);
		float getProperty(std::string propertyName);
		std::vector<std::string> getPropertyNames();
	private:
		ComponentID id = 0;
		EntityID entityID = DUMMY_ENTITY;
		std::unordered_map<std::string, size_t> propertyIndices;
		std::shared_ptr<ComponentPropertyPool> componentPropertyPool = nullptr;
    };
}