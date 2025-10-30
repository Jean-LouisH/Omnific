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

#include "world_environment.hpp"
#include <memory>

void Omnific::WorldEnvironment::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "clear_colour")
		{
			this->clear_colour = std::make_shared<Colour>(it3->second.as<std::string>());
		}
		else if (it3->first.as<std::string>() == "background_mode")
		{
			if (it3->second.as<std::string>() == "sky")
			{
				this->background_mode = BackgroundMode::SKY;
			}
			else if (it3->second.as<std::string>() == "clear_colour")
			{
				this->background_mode = BackgroundMode::CLEAR_COLOUR;
			}
		}
		else if (it3->first.as<std::string>() == "tone_mapper")
		{
			if (it3->second.as<std::string>() == "linear")
			{
				this->tone_mapper = ToneMapper::LINEAR;
			}
			else if (it3->second.as<std::string>() == "agx")
			{
				this->tone_mapper = ToneMapper::AGX;
			}
			else if (it3->second.as<std::string>() == "aces")
			{
				this->tone_mapper = ToneMapper::ACES;
			}
		}
		else if (it3->first.as<std::string>() == "world_environment_shader")
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