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
#include <core/scene/scene.hpp>
#include <core/asset.hpp>
#include <memory>
#include <thread>
#include <queue>

namespace Omnia
{
	class OMNIA_ENGINE_API FileAccess
	{
	public:
		FileAccess(std::string executable_filepath);
		~FileAccess();
		void set_data_directory(std::string asset_directory);
		std::shared_ptr<Scene> load_scene(std::string filepath);
		std::string get_executable_file_path();
		std::string get_executable_name();
		std::string get_executable_directory_path();
		std::string get_data_directory_path();
		std::string get_file_name_without_extension(std::string filepath);
		std::string get_file_extension(std::string filepath);
		std::string get_path_before_file(std::string filepath);
		bool exists(std::string filepath);

		std::string read_string(std::string filepath, bool apply_data_directory = true);
		std::vector<uint8_t> read_binary(std::string filepath, bool apply_data_directory = true);
		void write_binary(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory = true);

		void request_asynchronous_binary_reading(std::string filepath, bool apply_data_directory = true);
		void request_asynchronous_binary_writing(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory = true);
		bool has_binary_been_read_asynchronously(std::string filepath, bool apply_data_directory = true);
		std::vector<uint8_t> fetch_asynchronously_read_binary(std::string filepath, bool apply_data_directory = true);

		template <class DerivedAsset>
		std::shared_ptr<DerivedAsset> load_asset_by_type(std::string filepath, bool apply_data_directory = true)
		{
			std::shared_ptr<DerivedAsset> derived_asset;
			std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);

			if (this->assets.count(filepath) != 0)
			{
				derived_asset = std::dynamic_pointer_cast<DerivedAsset>(this->assets.at(filepath));
			}
			else
			{
				derived_asset = std::shared_ptr<DerivedAsset>(new DerivedAsset(full_filepath));
				std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(derived_asset);
				if (!this->assets.count(asset->get_name()))
					this->assets.emplace(asset->get_name(), asset);
			}

			return derived_asset;
		}
	private:
		std::string executable_filepath;
		std::string data_directory;
		std::unordered_map<std::string, std::shared_ptr<Omnia::Asset>> assets;

		std::vector<std::thread*> io_threads;
		std::unordered_map<std::string, std::vector<uint8_t>> asynchronously_loaded_binaries;

		std::string get_filepath_with_data_directory(std::string filepath, bool apply_data_directory = true);
		void read_binary_asynchronously(std::string filepath, bool apply_data_directory);
		void write_binary_asynchronously(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory);
	};
}