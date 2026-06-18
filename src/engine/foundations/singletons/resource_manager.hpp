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

#pragma once

#include <foundations/aliases.hpp>
#include <foundations/resources/resource.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <unordered_map>
#include <string>
#include <engine_api.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API ResourceManager
	{
	public:
		static std::shared_ptr<Resource> load(std::string filepath);
		template <class DerivedResource>
		static std::shared_ptr<DerivedResource> load(std::string filepath)
		{
			std::shared_ptr<DerivedResource> derived_resource;
			std::string full_filepath = Platform::get_file_access().find_path(filepath);
			ResourceManager* resource_manager = get_instance();

			if (resource_manager->resource_cache.count(filepath) != 0)
			{
				derived_resource = std::dynamic_pointer_cast<DerivedResource>(resource_manager->resource_cache.at(filepath));
			}
			else
			{
				Platform::get_logger().write("Loading Resource from file: " + filepath);
				derived_resource = std::shared_ptr<DerivedResource>(new DerivedResource(full_filepath));
				std::shared_ptr<Resource> resource = std::static_pointer_cast<Resource>(derived_resource);
				if (!resource_manager->resource_cache.count(resource->get_name()))
					resource_manager->resource_cache.emplace(resource->get_name(), resource);
			}

			return derived_resource;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Omnific::Resource>> resource_cache;

		static ResourceManager* instance;
		static ResourceManager* get_instance();
	};
}