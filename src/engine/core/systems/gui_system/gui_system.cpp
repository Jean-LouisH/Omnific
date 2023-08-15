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

#include "gui_system.hpp"
#include "core/singletons/os/os.hpp"
#include <core/singletons/event_bus.hpp>
#include <SDL_ttf.h>

Omnia::GUISystem::~GUISystem()
{
	this->finalize();
}

void Omnia::GUISystem::initialize()
{
	this->isInitialized = true;
	OS::getLogger().write("Initialized GUI System.");
	if (TTF_Init() == -1)
		printf("TTF_Init: %s\n", TTF_GetError());
}

void Omnia::GUISystem::onEarly(std::shared_ptr<Scene> scene)
{
	Input& hid = OS::getInput();
	std::unordered_map<std::string, double> numbers;
	std::unordered_map<std::string, std::string> strings;

	if (hid.isDropFileDetected())
	{
		numbers.emplace((std::string)"drop_file_window_id", (double)hid.getDropFileWindowID());
		strings.emplace((std::string)"drop_file_path", hid.getDropFilePath());
		EventBus::publish("file dropped on window", numbers, strings);
	}
}

void Omnia::GUISystem::finalize()
{
	this->isInitialized = false;
}