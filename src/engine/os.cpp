#include "os.hpp"
#include <string>
#include "SDL.h"
#include "utilities/constants.hpp"

Lilliputian::OS::OS(const char* title, uint16_t width, uint16_t height, bool isFullscreen, const char* executableFilepath)
{
	this->_window = new Window(title, width, height, isFullscreen);
	this->_hid = new HumanInterfaceDevices();
	this->_fileAccess = new FileAccess(executableFilepath);
}

Lilliputian::OS::~OS()
{
	delete this->_window;
	delete this->_hid;
	delete this->_fileAccess;
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

void Lilliputian::OS::addGameControllerMappings()
{
	String dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	String mappingFilename = "gamecontrollerdb.txt";
	String mappingFilepath = dataDirectory + mappingFilename;

	SDL_GameControllerAddMappingsFromFile(mappingFilepath.c_str());
}