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

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include "os/human_interface_devices.hpp"

namespace Lilliputian
{
	class InputAPI
	{
	public:
		InputAPI();
		void initialize(HumanInterfaceDevices* hid);
		bool isOnPress(std::string inputCode);
		bool isOnPress(std::vector<std::string> inputCodes);
		bool isOnDoublePress(std::string inputCode, unsigned int timeInterval_ms);
		bool isOnDoublePress(std::vector<std::string> inputCodes, unsigned int timerInterval_ms);
		bool isOnRelease(std::string inputCode);
		bool isOnRelease(std::vector<std::string> inputCodes);
		bool isOnHold(std::string inputCode);
		bool isOnHold(std::vector<std::string> inputCodes);
	private:
		HumanInterfaceDevices* hid = nullptr;

		std::map<std::string, SDL_Keycode> keyboardEventsByString;
		std::map<std::string, HumanInterfaceDevices::ControllerButtonCode> controllerButtonsByString;
		std::map<std::string, HumanInterfaceDevices::ControllerAxisCode> controllerAxisEventsByString;
	};
}