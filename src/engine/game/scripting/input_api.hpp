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
		void insertActionInput(const char* actionName, InputCode inputCode);
		void replaceActionInput(const char* actionName, InputCode inputCode);
		void removeActionInput(const char* actionName, InputCode inputCode);
		void removeAllActionInputs(const char* actionName);
		Map<String, Vector<InputCode>> getActionInputs();
		bool isKeyOnPress(unsigned int keyCode);
		bool isKeyOnDoublePress(unsigned int keyCode, unsigned int timeInterval_ms);
		bool isKeyOnRelease(unsigned int keyCode);
		bool isKeyOnHold(unsigned int keyCode);
		bool isActionOnPress(const char* actionName);
		bool isActionOnDoublePress(const char* actionName, unsigned int timeInterval_ms);
		bool isActionOnRelease(const char* actionName);
		bool isActionOnHold(const char* actionName, unsigned int timeInterval_ms);
		float getAxisStrength(const char* axisActionName);
	private:
		HumanInterfaceDevices* hid = nullptr;
		Map<String, Vector<InputCode>> actionInputs;
	};
}