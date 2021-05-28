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

#include "haptic_system.hpp"
#include <SDL.h>

Lilliputian::HapticSystem::HapticSystem()
{
	hapticRequests.reserve(8);
}

void Lilliputian::HapticSystem::rumble(Vector<SDL_Haptic*> haptics)
{
	for (int i = 0; i < this->hapticRequests.size(); i++)
	{
		HapticRequest currentHapticRequest = this->hapticRequests.at(i);

		SDL_HapticRumblePlay(
			haptics.at(currentHapticRequest.controllerID),
			currentHapticRequest.strength_pct,
			currentHapticRequest.duration_ms);
	}
}

void Lilliputian::HapticSystem::process(
	Scene& scene,
	HumanInterfaceDevices& hid)
{
	//Todo: Populate requests from scene

	this->rumble(hid.getHaptics());
	hapticRequests.clear();
}