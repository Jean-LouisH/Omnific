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

#include "component.hpp"
#include <application/uid_generator.hpp>

Omnia::Component::Component()
{
	this->id = UIDGenerator::getNewUID();
}

void Omnia::Component::setEntityID(EntityID entityID)
{
	this->entityID = entityID;
}

void Omnia::Component::setComponentPropertyPool(std::shared_ptr<Omnia::ComponentPropertyPool> componentPropertyPool)
{

}

Omnia::ComponentID Omnia::Component::getID()
{
	return this->id;
}

Omnia::EntityID Omnia::Component::getEntityID()
{
	return this->entityID;
}

bool Omnia::Component::isAttachedToEntity()
{
	return this->getEntityID() != 0;
}

std::string Omnia::Component::getType() const
{
	return this->type;
}

bool Omnia::Component::isType(std::string typeString)
{
	return this->type == typeString;
}

bool Omnia::Component::isRenderable()
{
	return false;
}

void Omnia::Component::addProperty(std::string propertyName, float propertyValue)
{

}

void Omnia::Component::updateProperty(std::string propertyName, float propertyValue)
{

}

float Omnia::Component::getProperty(std::string propertyName)
{
	float propertyValue = 0.0;

	return propertyValue;
}

std::vector<std::string> Omnia::Component::getPropertyNames()
{
	std::vector<std::string> names;
	
	return names;
}