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
#include "utilities/string.hpp"
#include "os/human_interface_devices.hpp"

namespace Lilliputian
{
	class InputAPI
	{
	public:
		uint16_t analogueStickSensitivity;

		InputAPI();
		void initialize(HumanInterfaceDevices* hid);
		void insertActionButtonMap(String actionName, String inputString);
		void replaceActionButtonMap(String actionName, String inputString);
		void removeActionButtonMap(String actionName, String inputString);
		void removeAllActionButtonMaps(String actionName);
		void insertActionAxisMap(String actionName, String negativeInputString, String positiveInputString);
		void insertActionAxisMap(String actionName, String inputString);
		void replaceActionAxisMap(String actionName, String negativeInputString, String positiveInputString);
		void removeActionAxisMap(String actionName, String negativeInputString, String positiveInputString);
		void removeAllActionAxisMaps(String actionName);
		std::map<String, std::vector<String>> getActionButtonMaps();
		std::map<String, std::vector<std::pair<String, String>>> getActionAxisMaps();
		bool isOnPress(String inputCode);
		bool isOnDoublePress(String keyCode, unsigned int timeInterval_ms);
		bool isOnRelease(String keyCode);
		bool isOnHold(String keyCode);
		bool isActionOnPress(String actionName);
		bool isActionOnDoublePress(String actionName, unsigned int timeInterval_ms);
		bool isActionOnRelease(String actionName);
		bool isActionOnHold(String actionName, unsigned int timeInterval_ms);
		float getActionStrength(String inputString);
		float getAxisActionStrength(String axisActionName);
	private:
		HumanInterfaceDevices* hid = nullptr;
		std::map<String, std::vector<String>> actionMappedButtons;
		std::map<String, std::vector<std::pair<String, String>>> actionMappedAxes;

		std::map<String, SDL_Keycode> keyboardEventsByString;
		std::map<String, HumanInterfaceDevices::ControllerButtonCode> controllerButtonsByString;
		std::map<String, HumanInterfaceDevices::ControllerAxisCode> controllerAxisEventsByString;
	};
}