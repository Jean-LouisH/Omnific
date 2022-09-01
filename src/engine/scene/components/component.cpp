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
#include <uid_generator.hpp>

Omnia::Component::Component()
{
	this->id = UIDGenerator::getNewUID();
	this->type = TYPE_STRING;
}

Omnia::Component::PropertyType Omnia::Component::queryPropertyType(std::string propertyName)
{
	return Omnia::Component::PropertyType::NONE;
}

void Omnia::Component::loadBoolProperty(std::string propertyName, std::vector<bool> propertyValue)
{

}

void Omnia::Component::loadIntProperty(std::string propertyName, std::vector<int> propertyValue)
{

}

void Omnia::Component::loadDoubleProperty(std::string propertyName, std::vector<double> propertyValue)
{

}

void Omnia::Component::loadStringProperty(std::string propertyName, std::vector<std::string> propertyValue)
{

}

void Omnia::Component::setEntityID(EntityID entityID)
{
	this->entityID = entityID;
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

bool Omnia::Component::isRenderable()
{
	return false;
}