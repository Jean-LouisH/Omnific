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
#include <asset_pipeline/assets/image.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <omnia_engine_api.hpp>

namespace Omnia
{
	/* Base class for objects that store and manipulate 
	   data relevant to individual Entities in a Scene.
	   These are attached to Entities when added to a Scene.*/
    class OMNIA_ENGINE_API Component
    {
	public:
		enum class PropertyType
		{
			NONE,
			BOOL,
			INT,
			DOUBLE,
			STRING
		};

		Component();

		virtual PropertyType queryPropertyType(std::string propertyName);
		virtual void loadBoolProperty(std::string propertyName, std::vector<bool> propertyValue);
		virtual void loadIntProperty(std::string propertyName, std::vector<int> propertyValue);
		virtual void loadDoubleProperty(std::string propertyName, std::vector<double> propertyValue);
		virtual void loadStringProperty(std::string propertyName, std::vector<std::string> propertyValue);

		void setEntityID(EntityID entityID);

		ComponentID getID();
		EntityID getEntityID();
		bool isAttachedToEntity();
		std::string getType() const;
		bool isType(std::string typeString);
		virtual bool isRenderable();
	protected:
		std::string type;
	private:
		ComponentID id = 0;
		EntityID entityID = 0;
    };
}