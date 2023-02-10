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
#include <core/singletons/os/os.hpp>

void Omnia::RenderableComponent::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "vertex shaders")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<Shader> vertexShader( new Shader(
					it3->second[i].as<std::string>(), 
					Shader::ShaderType::VERTEX)
				);
				this->addShader(vertexShader);
			}
		}
		else if (it3->first.as<std::string>() == "fragment shaders")
		{
			for (int i = 0; i < it3->second.size(); i++)
			{
				std::shared_ptr<Shader> fragmentShader(new Shader(
					it3->second[i].as<std::string>(),
					Shader::ShaderType::FRAGMENT)
				);
				this->addShader(fragmentShader);
			}
		}
		else if (it3->first.as<std::string>() == "dimensions")
		{
			this->setDimensions(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>());
		}
		else if (it3->first.as<std::string>() == "alpha")
		{
			this->setAlpha((uint8_t)(it3->second.as<double>() * 255.0));
		}
	}
}

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

void Omnia::RenderableComponent::setToNoFaceCulling()
{
	this->cullMode = CullMode::NONE;
}

void Omnia::RenderableComponent::setToFrontFaceCulling()
{
	this->cullMode = CullMode::FRONT;
}

void Omnia::RenderableComponent::setToBackFaceCulling()
{
	this->cullMode = CullMode::BACK;
}

void Omnia::RenderableComponent::setToFrontAndBackFaceCulling()
{
	this->cullMode = CullMode::FRONT_AND_BACK;
}

uint8_t Omnia::RenderableComponent::getAlpha()
{
	return this->alpha;
}

float Omnia::RenderableComponent::getAlphaInPercentage()
{
	return (this->alpha / 255.0);
}

bool Omnia::RenderableComponent::isNoFaceCulling()
{
	return this->cullMode == CullMode::NONE;
}

bool Omnia::RenderableComponent::isFrontFaceCulling()
{
	return this->cullMode == CullMode::FRONT;
}

bool Omnia::RenderableComponent::isBackFaceCulling()
{
	return this->cullMode == CullMode::BACK;
}

bool Omnia::RenderableComponent::isFrontAndBackFaceCulling()
{
	return this->cullMode == CullMode::FRONT_AND_BACK;
}

void Omnia::RenderableComponent::hide()
{
	this->alpha = 0;
}

void Omnia::RenderableComponent::show()
{
	this->alpha = 255;
}

Omnia::RenderableComponent::CullMode Omnia::RenderableComponent::getCullMode()
{
	return this->cullMode;
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