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

#include "file_access.hpp"
#include <SDL.h>
#include <fstream>
#include <algorithm>
#include <core/singletons/os/os.hpp>

Omnia::FileAccess::FileAccess(std::string executable_filepath)
{
	this->executable_filepath = executable_filepath;
}

Omnia::FileAccess::~FileAccess()
{
	for (std::thread* file_loading_thread : this->io_threads)
		file_loading_thread->join();
}

void Omnia::FileAccess::set_data_directory(std::string asset_directory)
{
	this->data_directory = asset_directory;
	OS::get_logger().write("Data Directory set to \"" + asset_directory + "\"");
}

std::shared_ptr<Omnia::Scene> Omnia::FileAccess::load_scene(std::string filepath)
{
	std::shared_ptr<Scene> scene;
	return scene;
}

std::string Omnia::FileAccess::get_executable_file_path()
{
	return this->executable_filepath;
}

std::string Omnia::FileAccess::get_executable_name()
{
	return this->get_file_name_without_extension(this->executable_filepath);
}

std::string Omnia::FileAccess::get_executable_directory_path()
{
	return this->get_path_before_file(this->executable_filepath);
}

std::string Omnia::FileAccess::get_data_directory_path()
{
	return this->data_directory;
}

std::string Omnia::FileAccess::get_file_name_without_extension(std::string filepath)
{
	std::string file_name;
	int name_index_start = filepath.size() - 1;

	if (name_index_start > -1)
	{
		//Find the first slash from the end
		while (filepath.at(name_index_start) != '/' && filepath.at(name_index_start) != '\\')
		{
			name_index_start--;
			if (name_index_start < 0)
			{
				break;
			}
		}
	}

	//append from the start index to the extension name.
	for (int j = name_index_start + 1; filepath.at(j) != '.';	j++)
		file_name += filepath.at(j);

	return file_name;
}

std::string Omnia::FileAccess::get_file_extension(std::string filepath)
{
	std::string file_name;
	int name_index_start = 0;

	//Find the first slash from the end
	for (name_index_start = filepath.size() - 1;
		filepath.at(name_index_start) != '.';
		name_index_start--)
		;

	//append from the start index to the extension name.
	for (int j = name_index_start + 1; filepath.at(j) != '\0'; j++)
		file_name += filepath.at(j);

	return file_name;
}

std::string Omnia::FileAccess::get_path_before_file(std::string filepath)
{
	std::string path;
	int name_end_index = filepath.size() - 1;

	//Find the first slash from the end
	if (name_end_index > -1)
	{
		while ((filepath.at(name_end_index) != '/' && filepath.at(name_end_index) != '\\'))
		{
			name_end_index--;
			if (name_end_index < 0)
			{
				break;
			}
		}
	}

	if (name_end_index > -1)
	{
		//append from the start index to the extension name.
		for (int j = 0; j != name_end_index; j++)
		{
			path += filepath.at(j);
		}
	}

	return path;
}

bool Omnia::FileAccess::exists(std::string filepath)
{
	std::ifstream input_file(filepath);
	return input_file.good();
}

std::string Omnia::FileAccess::read_string(std::string filepath, bool apply_data_directory)
{
	std::string output_string;
	std::vector<uint8_t> binary = this->read_binary(filepath, apply_data_directory);
	output_string.assign(binary.begin(), binary.end());
	return output_string;
}

std::vector<uint8_t> Omnia::FileAccess::read_binary(std::string filepath, bool apply_data_directory)
{
	std::vector<uint8_t> output_binary;
	std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);
	std::FILE* file_pointer = std::fopen(full_filepath.c_str(), "rb");

	if (file_pointer)
	{
		std::fseek(file_pointer, 0, SEEK_END);
		output_binary.resize(std::ftell(file_pointer));
		std::rewind(file_pointer);
		std::fread(&output_binary[0], 1, output_binary.size(), file_pointer);
		std::fclose(file_pointer);
	}

	return output_binary;
}

void Omnia::FileAccess::write_binary(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory)
{

}

void Omnia::FileAccess::request_asynchronous_binary_reading(std::string filepath, bool apply_data_directory)
{
	std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);

	if (!this->asynchronously_loaded_binaries.count(full_filepath))
		this->io_threads.push_back(new std::thread(&FileAccess::read_binary_asynchronously, this, filepath, apply_data_directory));
}

void Omnia::FileAccess::request_asynchronous_binary_writing(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory)
{
	std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);
	this->io_threads.push_back(new std::thread(&FileAccess::write_binary_asynchronously, this, filepath, binary, apply_data_directory));
}

bool Omnia::FileAccess::has_binary_been_read_asynchronously(std::string filepath, bool apply_data_directory)
{
	std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);
	return this->asynchronously_loaded_binaries.count(full_filepath);
}

std::vector<uint8_t> Omnia::FileAccess::fetch_asynchronously_read_binary(std::string filepath, bool apply_data_directory)
{
	std::string full_filepath = this->get_filepath_with_data_directory(filepath, apply_data_directory);
	std::vector<uint8_t> binary;
	if (this->asynchronously_loaded_binaries.count(full_filepath))
		binary = this->asynchronously_loaded_binaries.at(full_filepath);
	return binary;
}

std::string Omnia::FileAccess::get_filepath_with_data_directory(std::string filepath, bool apply_data_directory)
{
	std::string full_filepath;

	if (apply_data_directory)
		full_filepath = this->get_data_directory_path() + filepath;
	else
		full_filepath = filepath;

	return full_filepath;
}

void Omnia::FileAccess::read_binary_asynchronously(std::string filepath, bool apply_data_directory)
{
	std::vector<uint8_t> binary = this->read_binary(filepath, apply_data_directory);
	this->asynchronously_loaded_binaries.emplace(filepath, binary);
}

void Omnia::FileAccess::write_binary_asynchronously(std::string filepath, std::vector<uint8_t> binary, bool apply_data_directory)
{
	this->write_binary(filepath, binary, apply_data_directory);
}