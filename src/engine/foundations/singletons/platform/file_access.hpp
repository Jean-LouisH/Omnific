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
		std::string find_path(std::string filepath);
		std::string get_executable_file_path();
		std::string get_executable_name();
		std::string get_executable_directory_path();
		std::string get_file_name(std::string filepath);
		std::string get_file_name_without_extension(std::string filepath);
		std::string get_file_extension(std::string filepath);
		std::string get_path_before_file(std::string filepath);
		std::string get_last_modified_time(std::string filepath);
		bool exists(std::string filepath);
		bool is_file_type(std::string filepath, std::string file_type);

		std::string read_string(std::string filepath);
		std::vector<uint8_t> read_binary(std::string filepath);
		void write_binary(std::string filepath, std::vector<uint8_t> binary);

		void request_asynchronous_binary_reading(std::string filepath);
		void request_asynchronous_binary_writing(std::string filepath, std::vector<uint8_t> binary);
		bool has_binary_been_read_asynchronously(std::string filepath);
		std::vector<uint8_t> fetch_asynchronously_read_binary(std::string filepath);
	private:
		std::string executable_filepath;
		std::vector<std::string> app_data_paths;

		std::vector<std::thread*> io_threads;
		std::unordered_map<std::string, std::vector<uint8_t>> asynchronously_loaded_binaries;

		std::unordered_map<std::string, std::vector<uint8_t>> file_type_magic_numbers = {
			{"png", {0x89, 0x50, 0x4E, 0x47}},
			{"jpg", {0xFF, 0xD8, 0xFF}},
			{"jpeg", {0xFF, 0xD8, 0xFF}},
			{"bmp", {0x42, 0x4D}},
			{"gif", {0x47, 0x49, 0x46, 0x38}},
			{"webp", {0x52, 0x49, 0x46, 0x46}},
			{"tga", {0x00, 0x00, 0x02, 0x00}},
			{"ogg", {0x4F, 0x67, 0x67, 0x53}},
			{"wav", {0x52, 0x49, 0x46, 0x46}},
			{"ttf", {0x00, 0x01, 0x00, 0x00}},
			{"sqlite", {0x53, 0x51, 0x4C, 0x69}},
			{"zip", {0x50, 0x4B, 0x03, 0x04}},
			{"rar", {0x52, 0x61, 0x72, 0x21}},
			{"7z", {0x37, 0x7A, 0xBC, 0xAF}},
			{"pdf", {0x25, 0x50, 0x44, 0x46}},
			{"exe", {0x4D, 0x5A}},
			{"dll", {0x4D, 0x5A}},
			{"elf", {0x7F, 0x45, 0x4C, 0x46}},
			{"mp3", {0x49, 0x44, 0x33}},
			{"mp4", {0x00, 0x00, 0x00, 0x18}},
			{"avi", {0x52, 0x49, 0x46, 0x46}},
			{"mkv", {0x1A, 0x45, 0xDF, 0xA3}},
			{"midi", {0x4D, 0x54, 0x68, 0x64}},
			{"flac", {0x66, 0x4C, 0x61, 0x43}},
			{"aac", {0xFF, 0xF1}},
			{"flv", {0x46, 0x4C, 0x56}},
			{"swf", {0x43, 0x57, 0x53}},
			{"psd", {0x38, 0x42, 0x50, 0x53}},
			{"tar", {0x75, 0x73, 0x74, 0x61}},
			{"gz", {0x1F, 0x8B}},
			{"bz2", {0x42, 0x5A, 0x68}},
			{"kra", {0x4B, 0x52, 0x41}},
			{"fbx", {0x46, 0x42, 0x58}},
			{"obj", {0x6F, 0x62, 0x6A}},
			{"dae", {0x3C, 0x3F, 0x78, 0x6D}},
			{"gltf", {0x7B, 0x22, 0x61, 0x73}},
			{"glb", {0x67, 0x6C, 0x54}},
			{"blend", {0x42, 0x4C, 0x45, 0x4E}},
			{"wasm", {0x00, 0x61, 0x73, 0x6D}},
			{"m3u8", {0x23, 0x45, 0x58, 0x54}},
			{"csv", {0x76, 0x69, 0x6E, 0x63}},
			{"json", {0x7B, 0x22, 0x61, 0x73}},
			{"xml", {0x3C, 0x3F, 0x78, 0x6D}},
			{"yaml", {0x2D, 0x2D, 0x20}},
			{"ini", {0x5B, 0x5A, 0x45, 0x52}}
		};

		void read_binary_asynchronously(std::string filepath);
		void write_binary_asynchronously(std::string filepath, std::vector<uint8_t> binary);
		void log_loading_resource_from_file(std::string filepath);
	};
}