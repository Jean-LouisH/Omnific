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

#include "scene/assets/image.hpp"
#include "scene/renderable_component.hpp"
#include "scene/component.hpp"
#include "utilities/colour.hpp"

namespace Omnia
{
	/* A clickable region that is either 
	textured or overlayed to a parent Entity */

	class OMNIA_ENGINE_API UIButton : public RenderableComponent
	{
		friend class UISystem;
	public:
		UIButton()
		{
			this->type = TYPE_STRING;
			this->defaultDimensions = { 20, 10 };
		};
		static constexpr const char* TYPE_STRING = "UIButton";

		void enableOverlayToParent();
		void disableOverlayToParent();
		void setToColour(std::shared_ptr<Colour> colour);
		void setToImage(std::shared_ptr<Image> image);
	private:
		bool isPressed = false;
		bool isOverlayedToParent = false;
		glm::vec2 defaultDimensions;
	};
}