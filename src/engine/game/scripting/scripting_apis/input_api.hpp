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
		uint16_t analogueStickSensitivity;

		InputAPI();
		void initialize(HumanInterfaceDevices* hid);
		void insertActionButtonMap(std::string actionName, std::string inputString);
		void replaceActionButtonMap(std::string actionName, std::string inputString);
		void removeActionButtonMap(std::string actionName, std::string inputString);
		void removeAllActionButtonMaps(std::string actionName);
		void insertActionAxisMap(std::string actionName, std::string negativeInputString, std::string positiveInputString);
		void insertActionAxisMap(std::string actionName, std::string inputString);
		void replaceActionAxisMap(std::string actionName, std::string negativeInputString, std::string positiveInputString);
		void removeActionAxisMap(std::string actionName, std::string negativeInputString, std::string positiveInputString);
		void removeAllActionAxisMaps(std::string actionName);
		std::map<std::string, std::vector<std::string>> getActionButtonMaps();
		std::map<std::string, std::vector<std::pair<std::string, std::string>>> getActionAxisMaps();
		bool isOnPress(std::string inputCode);
		bool isOnDoublePress(std::string keyCode, unsigned int timeInterval_ms);
		bool isOnRelease(std::string keyCode);
		bool isOnHold(std::string keyCode);
		bool isActionOnPress(std::string actionName);
		bool isActionOnDoublePress(std::string actionName, unsigned int timeInterval_ms);
		bool isActionOnRelease(std::string actionName);
		bool isActionOnHold(std::string actionName, unsigned int timeInterval_ms);
		float getActionStrength(std::string inputString);
		float getAxisActionStrength(std::string axisActionName);
	private:
		HumanInterfaceDevices* hid = nullptr;
		std::map<std::string, std::vector<std::string>> actionMappedButtons;
		std::map<std::string, std::vector<std::pair<std::string, std::string>>> actionMappedAxes;

		std::map<std::string, SDL_Keycode> keyboardEventsByString;
		std::map<std::string, HumanInterfaceDevices::ControllerButtonCode> controllerButtonsByString;
		std::map<std::string, HumanInterfaceDevices::ControllerAxisCode> controllerAxisEventsByString;
	};
}