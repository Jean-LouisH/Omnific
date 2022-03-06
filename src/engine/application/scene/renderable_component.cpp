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

#include "renderable_component.hpp"

void Omnia::RenderableComponent::setDimensions(float width, float height)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
}

void Omnia::RenderableComponent::setDimensions(float width, float height, float depth)
{
	this->dimensions.x = width;
	this->dimensions.y = height;
	this->dimensions.z = depth;
}

void Omnia::RenderableComponent::addShader(std::shared_ptr<Omnia::Shader> shader)
{
	this->shaders.push_back(shader);
}

void Omnia::RenderableComponent::setAlpha(uint8_t value)
{
	this->alpha = value;
}

uint8_t Omnia::RenderableComponent::getAlpha()
{
	return this->alpha;
}

float Omnia::RenderableComponent::getAlphaInPercentage()
{
	return (this->alpha / 255.0);
}

void Omnia::RenderableComponent::hide()
{
	this->alpha = 0;
}

void Omnia::RenderableComponent::show()
{
	this->alpha = 255;
}

std::shared_ptr<Omnia::Image> Omnia::RenderableComponent::getImage()
{
	return this->image;
}

std::vector<std::shared_ptr<Omnia::Shader>> Omnia::RenderableComponent::getShaders()
{
	return this->shaders;
}

glm::vec3 Omnia::RenderableComponent::getDimensions()
{
	return this->dimensions;
}

bool Omnia::RenderableComponent::isRenderable()
{
	return true;
}