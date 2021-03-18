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