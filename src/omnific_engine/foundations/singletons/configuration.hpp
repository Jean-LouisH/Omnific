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
#include <stdint.h>

namespace Omnific 
{
	class OMNIFIC_ENGINE_API Configuration
	{
	public:
		struct Metadata
		{
			std::string title;
			std::string version;
			std::string developer;
			std::string icon_filepath;
			std::string entry_scene_filepath;
		} metadata;

		struct WindowSettings
		{
			uint16_t height;
			uint16_t width;
			bool is_starting_fullscreen;
			bool is_starting_maximized;
			bool is_resizable;
		} window_settings;

		struct TimeSettings
		{
			uint32_t ms_per_compute_update;
			uint32_t target_fps;
		} time_settings;

		//Status
		bool is_loaded = false;

		static void load_from_file(std::string boot_filepath);

		static Configuration* get_instance();
	private:
		static Configuration* instance;
	};
}