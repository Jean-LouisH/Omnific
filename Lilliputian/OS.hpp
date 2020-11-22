#pragma once

#include <SDL.h>
#include "Utilities/Collections/Vector.hpp"
#include "HumanInterfaceDevices.hpp"
#include "Window.hpp"
#include "FileAccess.hpp"

namespace Lilliputian
{
	class OS
	{
	public:
		OS(const char* title, uint16_t width, uint16_t height, bool isFullscreen, const char* executableFilepath);
		Window& window();
		HumanInterfaceDevices& hid();
		FileAccess& fileAccess();
	private:
		Window* _window = nullptr;
		HumanInterfaceDevices* _hid = nullptr;
		FileAccess* _fileAccess = nullptr;
	};
}