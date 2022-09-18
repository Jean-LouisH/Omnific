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
#include <customization/classes/assets/image.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <omnia_engine_api.hpp>
#include <yaml-cpp/yaml.h>
#include <customization/class_registry/registerable.hpp>

namespace Omnia
{
	/* Base class for objects that store and manipulate 
	   data relevant to individual Entities in a Scene.
	   These are attached to Entities when added to a Scene.*/
    class OMNIA_ENGINE_API Component : public Registerable
    {
	public:
		static constexpr const char* TYPE_STRING = "Component";

		Component();

		void setEntityID(EntityID entityID);
		void setComponentHierarchy(std::vector<std::shared_ptr<Component>> componentHierarchy);
		virtual void deserialize(YAML::Node yamlNode);

		EntityID getEntityID();
		bool isAttachedToEntity();
		virtual bool isRenderable();
	protected:
		std::vector<std::shared_ptr<Component>> componentHierarchy;
	private:
		EntityID entityID = 0;
    };
}