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