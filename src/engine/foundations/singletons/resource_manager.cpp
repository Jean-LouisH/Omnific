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

#include "foundations/singletons/resource_manager.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/resources/audio.hpp>
#include <foundations/resources/font.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/resources/script.hpp>
#include <foundations/resources/shader.hpp>
#include <memory>

Omnific::ResourceManager* Omnific::ResourceManager::instance = nullptr;

std::shared_ptr<Omnific::Resource> Omnific::ResourceManager::load_resource(std::string filepath)
{
	std::shared_ptr<Resource> resource;
	ResourceManager* resource_manager = get_instance();
	FileAccess& file_access = Platform::get_file_access();
	std::string full_filepath = file_access.find_path(filepath);

	if (resource_manager->resource_cache.count(filepath) != 0)
	{
		resource = resource_manager->resource_cache.at(filepath);
	}
	else
	{
		if (file_access.exists(full_filepath))
		{
			Platform::get_logger().write("Loading Resource from file: " + filepath);

			if (file_access.is_file_type(full_filepath, "ogg") || 
				file_access.is_file_type(full_filepath, "wav") || 
				file_access.get_file_extension(full_filepath) == "libretti" || 
				file_access.get_file_extension(full_filepath) == "txt")
			{
				resource = std::dynamic_pointer_cast<Audio>(std::make_shared<Audio>(full_filepath));
			}
			else if (file_access.is_file_type(full_filepath, "ttf"))
			{
				resource = std::dynamic_pointer_cast<Font>(std::make_shared<Font>(full_filepath));
			}
			else if (file_access.is_file_type(full_filepath, "jpg") || 
				file_access.is_file_type(full_filepath, "png"))
			{
				resource = std::dynamic_pointer_cast<Image>(std::make_shared<Image>(full_filepath));
			}
			else if (file_access.is_file_type(full_filepath, "glb") || 
					file_access.get_file_extension(full_filepath) == "yaml")
			{
				resource = std::dynamic_pointer_cast<Scene>(std::make_shared<Scene>(full_filepath));
			}
			else if (file_access.get_file_extension(full_filepath) == "py")
			{
				resource = std::dynamic_pointer_cast<Script>(std::make_shared<Script>(full_filepath));
			}
			else if (file_access.get_file_extension(full_filepath) == "glsl")
			{
				resource = std::dynamic_pointer_cast<Shader>(std::make_shared<Shader>(full_filepath));
			}

			if (!resource_manager->resource_cache.count(resource->get_name()))
				resource_manager->resource_cache.emplace(resource->get_name(), resource);
		}
	}

	return resource;
}

Omnific::ResourceManager* Omnific::ResourceManager::get_instance()
{
	if (instance == nullptr)
		instance = new ResourceManager();
	return instance;
}