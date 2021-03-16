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