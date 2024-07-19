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

#include "foundations/singletons/configuration.hpp"
#include <foundations/singletons/platform/platform.hpp>

Omnific::Configuration* Omnific::Configuration::instance = nullptr;

#include <yaml-cpp/yaml.h>

void Omnific::Configuration::load_from_file(std::string boot_filepath)
{
	Configuration* configuration = Configuration::get_instance();

	try
	{
		YAML::Node yaml_node = YAML::LoadFile(boot_filepath);

		for (YAML::const_iterator it0 = yaml_node.begin(); it0 != yaml_node.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "metadata")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "title")
					{
						configuration->metadata.title = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "version")
					{
						configuration->metadata.version = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "developer")
					{
						configuration->metadata.developer = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "icon_filepath")
					{
						configuration->metadata.icon_filepath = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "entry_scene_filepath")
					{
						configuration->metadata.entry_scene_filepath = it1->second.as<std::string>();
					}
				}
			}
			else if (it0->first.as<std::string>() == "window_settings")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "width")
					{
						configuration->window_settings.width = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "height")
					{
						configuration->window_settings.height = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "fullscreen")
					{
						configuration->window_settings.is_starting_fullscreen = it1->second.as<bool>();
					}
					else if (it1->first.as<std::string>() == "maximized")
					{
						configuration->window_settings.is_starting_maximized = it1->second.as<bool>();
					}
					else if (it1->first.as<std::string>() == "resizable")
					{
						configuration->window_settings.is_resizable = it1->second.as<bool>();
					}
				}
			}
			else if (it0->first.as<std::string>() == "time_settings")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "target_fps")
					{
						configuration->time_settings.target_fps = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "ms_per_compute_update")
					{
						configuration->time_settings.ms_per_compute_update = it1->second.as<int>();
					}
				}
			}
		}

		configuration->is_loaded = true;

		Platform::get_logger().write("Successfully loaded Configuration.");
	}
	catch (int e)
	{
		configuration->is_loaded = false;
	}
}

Omnific::Configuration* Omnific::Configuration::get_instance()
{
	if (instance == nullptr)
		instance = new Configuration();
	return instance;
}