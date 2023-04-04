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

#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/singletons/os/os.hpp>

void Omnia::Camera::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "viewport_px")
		{
			this->setViewportHeight(it3->second[1].as<double>());
			this->setViewportWidth(it3->second[0].as<double>());
		}
		else if (it3->first.as<std::string>() == "limits_px")
		{
			this->setLimits(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>(),
				it3->second[3].as<double>()
			);
		}
		else if (it3->first.as<std::string>() == "keepAspect")
		{
			this->setKeepAspect(it3->second.as<bool>());
		}
		else if (it3->first.as<std::string>() == "isStreaming")
		{
			this->setIsStreaming(it3->second.as<bool>());
		}
	}
}

void Omnia::Camera::resetAspect()
{
	this->aspect = defaultAspect;
}

void Omnia::Camera::toggleWireframeMode()
{
	if (this->getIsWireframeMode())
		this->setWireframeMode(false);
	else
		this->setWireframeMode(true);
}

glm::vec2 Omnia::Camera::getViewport()
{
	return glm::vec2(this->viewport.x, this->viewport.y);
}

bool Omnia::Camera::getIsStreaming()
{
	return this->isStreaming;
}

bool Omnia::Camera::getIsWireframeMode()
{
	return this->isWireFrame;
}

void Omnia::Camera::setViewportWidth(uint32_t width_px)
{
	this->viewport.x = width_px;
	if (keepAspect)
		this->viewport.y = (uint32_t)((double)width_px / aspect);
}

void Omnia::Camera::setViewportHeight(uint32_t height_px)
{
	this->viewport.y = height_px;
	if (keepAspect)
		this->viewport.x = (uint32_t)((double)height_px * aspect);
}

void Omnia::Camera::setLimits(float left, float bottom, float top, float right)
{
	this->limits.min.x = left;
	this->limits.min.y = bottom;
	this->limits.max.x = right;
	this->limits.max.y = top;
}

void Omnia::Camera::setKeepAspect(bool value)
{
	this->keepAspect = value;
}

void Omnia::Camera::setIsStreaming(bool value)
{
	this->isStreaming = value;
}

void Omnia::Camera::setWireframeMode(bool value)
{
	this->isWireFrame = value;
}

glm::mat4 Omnia::Camera::getViewToProjectionMatrix()
{
	glm::vec2 windowSize = OS::getWindow().getWindowSize();
	return glm::perspective(glm::radians(this->fieldOfView), (float)windowSize.x / (float)windowSize.y, this->nearPlane, this->farPlane);
}