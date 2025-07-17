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

#include "light.hpp"

void Omnific::Light::deserialize(YAML::Node yaml_node)
{

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		std::string value = it3->first.as<std::string>();

		if (value == "light_mode")
		{
			std::string light_mode_value = it3->second.as<std::string>();

			if (light_mode_value == "directional")
			{
				this->mode = LightMode::DIRECTIONAL;
			}
			else if (light_mode_value == "point")
			{
				this->mode = LightMode::POINT;
			}
			else if (light_mode_value == "spot")
			{
				this->mode = LightMode::SPOT;
			}
		}
		else if (value == "colour")
		{
			this->colour = std::shared_ptr<Colour>(new Colour(it3->second.as<std::string>()));
		}
		else if (value == "shadow_colour")
		{
			this->shadow_colour = std::shared_ptr<Colour>(new Colour(it3->second.as<std::string>()));
		}
		else if (value == "intensity")
		{
			this->intensity = it3->second.as<float>();
		}
		else if (value == "range")
		{
			this->range = it3->second.as<float>();
		}
		else if (value == "angle")
		{
			this->inner_cutoff_angle = it3->second.as<float>();
		}
		else if (value == "is_shadow_enabled")
		{
			this->is_shadow_enabled = it3->second.as<bool>();
		}
	}
}