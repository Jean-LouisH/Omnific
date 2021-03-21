//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "boot_loader.hpp"
#include <yaml-cpp/yaml.h>

Lilliputian::BootConfiguration* Lilliputian::BootLoader::loadFromFile(String bootFilepath)
{
	BootConfiguration* configuration = new BootConfiguration();

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(bootFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "game")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "title")
					{
						configuration->gameTitle = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "version")
					{
						configuration->version = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "developer")
					{
						configuration->developer = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "icon_filepath")
					{
						configuration->iconFilepath = it1->second.as<std::string>();
					}
					else if (it1->first.as<std::string>() == "entry_scene_filepath")
					{
						configuration->entrySceneFilepath = it1->second.as<std::string>();
					}
				}
			}
			else if (it0->first.as<std::string>() == "game_window")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "width")
					{
						configuration->windowWidth = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "height")
					{
						configuration->windowHeight = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "fullscreen")
					{
						configuration->isStartingFullscreen = it1->second.as<bool>();
					}
					else if (it1->first.as<std::string>() == "maximized")
					{
						configuration->isStartingMaximized = it1->second.as<bool>();
					}
					else if (it1->first.as<std::string>() == "resizable")
					{
						configuration->isResizable = it1->second.as<bool>();
					}
				}
			}
			else if (it0->first.as<std::string>() == "time")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "target_fps")
					{
						configuration->targetFPS = it1->second.as<int>();
					}
					else if (it1->first.as<std::string>() == "ms_per_compute_update")
					{
						configuration->msPerComputeUpdate = it1->second.as<int>();
					}
				}
			}
		}

		configuration->isLoaded = true;
	}
	catch (int e)
	{
		configuration->isLoaded = false;
	}

	return configuration;
}