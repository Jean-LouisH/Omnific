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

#pragma once

#include <SDL.h>
#include "utilities/collections/vector.hpp"
#include "human_interface_devices.hpp"
#include "window.hpp"
#include "file_access.hpp"

namespace Lilliputian
{
	class OS
	{
	public:
		OS(const char* title, uint16_t width, uint16_t height, bool isFullscreen, const char* executableFilepath);
		~OS();
		Window& window();
		HumanInterfaceDevices& hid();
		FileAccess& fileAccess();
		void addGameControllerMappings();
	private:
		Window* _window = nullptr;
		HumanInterfaceDevices* _hid = nullptr;
		FileAccess* _fileAccess = nullptr;
	};
}