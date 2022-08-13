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
#include "os/os.hpp"
#include <SDL_ttf.h>

Omnia::GUISystem::~GUISystem()
{
	this->deinitialize();
}

void Omnia::GUISystem::initialize()
{
	this->isInitialized = true;
}

void Omnia::GUISystem::onLogic(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& sceneTrees = scene->getSceneTrees();
	Input& hid = OS::getInput();
	std::unordered_map<std::string, double> numbers;
	std::unordered_map<std::string, std::string> strings;

	if (hid.isDropFileDetected())
	{
		numbers.emplace((std::string)"drop_file_window_id", (double)hid.getDropFileWindowID());
		strings.emplace((std::string)"drop_file_path", hid.getDropFilePath());
	}

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
	{
		if (hid.isDropFileDetected())
			it->second->getEventBus()->publish("file dropped on window", numbers, strings);
	}
}

void Omnia::GUISystem::deinitialize()
{
	this->isInitialized = false;
}

void Omnia::GUISystem::orderGUIComponentsByHierarchy()
{

}

void Omnia::GUISystem::positionGUIComponentsByHierarchy()
{

}

bool Omnia::GUISystem::isCursorHoveringOverGUIComponent(ComponentID componentID)
{
	return false;
}

//bool Omnia::GUISystem::isCursorInInputStateOnGUIComponent(
//	ComponentID componentID,
//	InputCode inputCode,
//	InputButtonMode inputButtonMode)
//{
//	return false;
//}