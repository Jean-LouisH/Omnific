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
#include "utilities/collections/map.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/string.hpp"
#include "utilities/input_code.hpp"
#include "os/human_interface_devices.hpp"

namespace Lilliputian
{
	class InputAPI
	{
	public:

		uint16_t analogueStickSensitivity;

		InputAPI();
		void initialize(HumanInterfaceDevices* hid);
		void insertActionInput(String actionName, InputCode inputCode);
		void replaceActionInput(String actionName, InputCode inputCode);
		void removeActionInput(String actionName, InputCode inputCode);
		void removeAllActionInputs(String actionName);
		Map<String, Vector<InputCode>> getActionInputs();
		bool isOnPress(String inputCode);
		bool isOnDoublePress(String keyCode, unsigned int timeInterval_ms);
		bool isOnRelease(String keyCode);
		bool isOnHold(String keyCode);
		bool isActionOnPress(String actionName);
		bool isActionOnDoublePress(String actionName, unsigned int timeInterval_ms);
		bool isActionOnRelease(String actionName);
		bool isActionOnHold(String actionName, unsigned int timeInterval_ms);
		float getAxisStrength(String axisActionName);
	private:
		HumanInterfaceDevices* hid = nullptr;
		Map<String, Vector<InputCode>> actionInputs;
		Map<String, InputCode> inputCodeStrings;
	};
}