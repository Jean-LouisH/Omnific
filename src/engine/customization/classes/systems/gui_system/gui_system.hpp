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

#include "scene/scene.hpp"
#include "system.hpp"
#include <memory>

namespace Omnia
{
	/* Processes Components with data related to HID interaction.*/
	class GUISystem : public System
	{
	public:
		GUISystem()
		{
			this->type = TYPE_STRING;
			this->threadType = ThreadType::UPDATE;
		};
		~GUISystem();

		static constexpr const char* TYPE_STRING = "GUISystem";

		virtual Registerable* instance() override
		{
			return new GUISystem(*this);
		}

		virtual void initialize() override;
		virtual void onLogic(std::shared_ptr<Scene> scene) override;
		virtual void finalize() override;
	private:
		void orderGUIComponentsByHierarchy();
		void positionGUIComponentsByHierarchy();
		bool isCursorHoveringOverGUIComponent(ComponentID componentID);
		//bool isCursorInInputStateOnGUIComponent(
		//	ComponentID componentID, 
		//	InputCode inputCode, 
		//	InputButtonMode inputButtonMode);
	};
}

