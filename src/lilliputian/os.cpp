#include "os.hpp"
#include <string>

Lilliputian::OS::OS(const char* title, uint16_t width, uint16_t height, bool isFullscreen, const char* executableFilepath)
{
	this->_window = new Window(title, width, height, isFullscreen);
	this->_hid = new HumanInterfaceDevices();
	this->_fileAccess = new FileAccess(executableFilepath);
}

Lilliputian::Window& Lilliputian::OS::window()
{
	return *this->_window;
}

Lilliputian::HumanInterfaceDevices& Lilliputian::OS::hid()
{
	return *this->_hid;
}

Lilliputian::FileAccess& Lilliputian::OS::fileAccess()
{
	return *this->_fileAccess;
}