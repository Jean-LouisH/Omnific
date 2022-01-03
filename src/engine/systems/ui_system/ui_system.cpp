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

#include "ui_system.hpp"
#include "SDL_ttf.h"
#include "os/os.hpp"

Esi::UISystem::UISystem()
{
	if (TTF_Init() == -1) 
		printf("TTF_Init: %s\n", TTF_GetError());
}

Esi::UISystem::~UISystem()
{
	TTF_Quit();
}

void Esi::UISystem::process(Scene& scene)
{

}

void Esi::UISystem::orderUIComponentsByHierarchy()
{

}

void Esi::UISystem::positionUIComponentsByHierarchy()
{

}

bool Esi::UISystem::isCursorHoveringOverUIComponent(ComponentID componentID)
{
	return false;
}

//bool Esi::UISystem::isCursorInInputStateOnUIComponent(
//	ComponentID componentID,
//	InputCode inputCode,
//	InputButtonMode inputButtonMode)
//{
//	return false;
//}