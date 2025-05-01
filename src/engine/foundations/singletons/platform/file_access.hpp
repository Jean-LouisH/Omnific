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

#include <string>
#include <engine_api.hpp>
#include <scene/scene.hpp>
#include <foundations/resources/resource.hpp>
#include <memory>
#include <thread>
#include <queue>

namespace Omnific
{
	class OMNIFIC_ENGINE_API FileAccess
	{
	public:
		FileAccess(std::string executable_filepath);
		~FileAccess();
		void add_app_data_paths(std::vector<std::string> app_data_paths);
		std::shared_ptr<Scene> load_scene(std::string filepath);
		std::string find_path(std::string filepath);
		std::string get_executable_file_path();
		std::string get_executable_name();
		std::string get_executable_directory_path();
		std::string get_file_name_without_extension(std::string filepath);
		std::string get_file_extension(std::string filepath);
		std::string get_path_before_file(std::string filepath);
		std::string get_last_modified_time(std::string filepath);
		bool exists(std::string filepath);

		std::string read_string(std::string filepath, bool apply_app_data_path = true);
		std::vector<uint8_t> read_binary(std::string filepath, bool apply_app_data_path = true);
		void write_binary(std::string filepath, std::vector<uint8_t> binary, bool apply_app_data_path = true);

		void request_asynchronous_binary_reading(std::string filepath, bool apply_app_data_path = true);
		void request_asynchronous_binary_writing(std::string filepath, std::vector<uint8_t> binary, bool apply_app_data_path = true);
		bool has_binary_been_read_asynchronously(std::string filepath, bool apply_app_data_path = true);
		std::vector<uint8_t> fetch_asynchronously_read_binary(std::string filepath, bool apply_app_data_path = true);

		template <class DerivedResource>
		std::shared_ptr<DerivedResource> load_resource_by_type(std::string filepath, bool apply_app_data_path = true)
		{
			std::shared_ptr<DerivedResource> derived_resource;
			std::string full_filepath = this->get_filepath_with_app_data_path(filepath, apply_app_data_path);

			if (this->resources.count(filepath) != 0)
			{
				derived_resource = std::dynamic_pointer_cast<DerivedResource>(this->resources.at(filepath));
			}
			else
			{
				derived_resource = std::shared_ptr<DerivedResource>(new DerivedResource(full_filepath));
				std::shared_ptr<Resource> resource = std::static_pointer_cast<Resource>(derived_resource);
				if (!this->resources.count(resource->get_name()))
					this->resources.emplace(resource->get_name(), resource);
			}

			return derived_resource;
		}
	private:
		std::string executable_filepath;
		std::vector<std::string> app_data_paths;
		std::unordered_map<std::string, std::shared_ptr<Omnific::Resource>> resources;

		std::vector<std::thread*> io_threads;
		std::unordered_map<std::string, std::vector<uint8_t>> asynchronously_loaded_binaries;

		std::string get_filepath_with_app_data_path(std::string filepath, bool apply_app_data_path = true);
		void read_binary_asynchronously(std::string filepath, bool apply_app_data_path);
		void write_binary_asynchronously(std::string filepath, std::vector<uint8_t> binary, bool apply_app_data_path);
	};
}