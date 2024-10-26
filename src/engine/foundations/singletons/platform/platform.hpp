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

#include <SDL.h>
#include <vector>
#include "shared_library_access.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "window.hpp"
#include "file_access.hpp"
#include "network_access.hpp"
#include <string>
#include <engine_api.hpp>

namespace Omnific
{

	class OMNIFIC_ENGINE_API Clock
	{
	public:
		/* Returns delta time in Milliseconds */
		uint64_t get_delta();
		float get_delta_in_seconds();
		void set_start();
		void set_end();
	private:
		uint64_t delta = 0; //In milliseconds.
		uint64_t start = 0;
		uint64_t finish = 0;
	};

	/* Singleton for objects related to Operating System utilities. */
	class OMNIFIC_ENGINE_API Platform
	{
	public:
		static void initialize(
			int argc,
			char* argv[]
		);

		static bool create_window(std::string title,
			uint16_t width,
			uint16_t height,
			bool is_fullscreen,
			std::string rendering_context);

		static void yield_thread();
		/* Duration in milliseconds */
		static void sleep_thread_for(int duration);

		static void show_error_box(std::string title, std::string message);

		static uint8_t get_logical_core_count();
		/* In Bytes */
		static uint32_t get_l1_cache_line_size();
		/* In MegaBytes */
		static uint32_t get_system_ram();
		static std::string get_platform_name();

		static void finalize();

		static SharedLibraryAccess& get_shared_library_access();
		static Window& get_window();
		static Input& get_input();
		static Logger& get_logger();
		static FileAccess& get_file_access();
		static NetworkAccess& get_network_access();

		static Clock& get_run_clock();
		static std::vector<std::string> get_args();
		static void add_game_controller_mappings(std::string mapping_filepath);
	private:
		static Platform* instance;

		std::vector<std::string> command_line_arguments;

		std::unique_ptr<SharedLibraryAccess> shared_library_access;
		std::unique_ptr<Clock> run_clock;
		std::unique_ptr<Window> window;
		std::unique_ptr<Input> input;
		std::unique_ptr<Logger> logger;
		std::unique_ptr<FileAccess> file_access;
		std::unique_ptr<NetworkAccess> network_access;

		static Platform* get_instance();
	};
}