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
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <foundations/singletons/platform/platform.hpp>

void Omnific::Camera::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "viewport_px")
		{
			this->set_viewport_height(it3->second[1].as<double>());
			this->set_viewport_width(it3->second[0].as<double>());
		}
		else if (it3->first.as<std::string>() == "limits_px")
		{
			this->set_limits(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>(),
				it3->second[3].as<double>()
			);
		}
		else if (it3->first.as<std::string>() == "keep_aspect")
		{
			this->set_keep_aspect(it3->second.as<bool>());
		}
		else if (it3->first.as<std::string>() == "is_streaming")
		{
			this->set_is_streaming(it3->second.as<bool>());
		}
		else if (it3->first.as<std::string>() == "wireframe_mode")
		{
			this->set_wireframe_mode(it3->second.as<bool>());
		}
		else if (it3->first.as<std::string>() == "follow_target_entity")
		{
			this->follow_target_entity = it3->second.as<std::string>();
		}
		else if (it3->first.as<std::string>() == "viewport_target_entity")
		{
			this->viewport_target_entity = it3->second.as<std::string>();
		}
		else if (it3->first.as<std::string>() == "camera_shader")
		{
			std::string vertex = "";
			std::string fragment = "";
			std::string preset = "Shader::PBR";

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "vertex")
				{
					vertex = it4->second.as<std::string>();
				}
				else if (it4->first.as<std::string>() == "fragment")
				{
					fragment = it4->second.as<std::string>();
				}
				else if (it4->first.as<std::string>() == "preset")
				{
					preset = it4->second.as<std::string>();
				}
			}

			if (preset == "")
				this->shader = std::make_shared<Shader>(vertex, fragment);
			else
				this->shader = std::make_shared<Shader>(preset);
		}
	}
}

void Omnific::Camera::reset_aspect()
{
	this->aspect = default_aspect;
}

void Omnific::Camera::toggle_wireframe_mode()
{
	if (this->get_is_wireframe_mode())
		this->set_wireframe_mode(false);
	else
		this->set_wireframe_mode(true);
}

glm::vec2 Omnific::Camera::get_viewport()
{
	return glm::vec2(this->viewport.x, this->viewport.y);
}

bool Omnific::Camera::get_is_streaming()
{
	return this->is_streaming;
}

bool Omnific::Camera::get_is_wireframe_mode()
{
	return this->is_wire_frame;
}

void Omnific::Camera::set_viewport_width(uint32_t width_px)
{
	this->viewport.x = width_px;
	if (keep_aspect)
		this->viewport.y = (uint32_t)((double)width_px / aspect);
}

void Omnific::Camera::set_viewport_height(uint32_t height_px)
{
	this->viewport.y = height_px;
	if (keep_aspect)
		this->viewport.x = (uint32_t)((double)height_px * aspect);
}

void Omnific::Camera::set_limits(float left, float bottom, float top, float right)
{
	this->min_limit.x = left;
	this->min_limit.y = bottom;
	this->max_limit.x = right;
	this->max_limit.y = top;
}

void Omnific::Camera::set_keep_aspect(bool value)
{
	this->keep_aspect = value;
}

void Omnific::Camera::set_is_streaming(bool value)
{
	this->is_streaming = value;
}

void Omnific::Camera::set_wireframe_mode(bool value)
{
	this->is_wire_frame = value;
}

glm::mat4 Omnific::Camera::get_view_to_projection_matrix()
{
	glm::vec2 window_size = Platform::get_window().get_window_size();
	return glm::perspective(glm::radians(this->field_of_view), (float)window_size.x / (float)window_size.y, this->near_plane, this->far_plane);
}