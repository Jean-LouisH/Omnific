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

#include "asset_pipeline/assets/image.hpp"
#include "scene/components/renderable_component.hpp"
#include "scene/components/component.hpp"
#include "utilities/colour.hpp"
#include <unordered_map>
#include <string>

namespace Omnia
{
	class GUIText
	{
	public:
		GUIText()
		{
			this->image = std::shared_ptr<Image>(new Image());
		};

		void setText(std::string text);
		void setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px);
		void setFontStyleNormal();
		void setFontStyleBold();
		void setFontStyleItalic();
		void setFontStyleUnderline();
		void setFontStyleStrikethrough();
		void setFontRenderModeSolid();
		void setFontRenderModeShaded();
		void setFontRenderModeBlended();
		void setFontSize(uint16_t size_px);
		void setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		Font::Style getFontStyle();

		std::string text;
		std::shared_ptr<Omnia::Font> font;
		Font::Style style = Font::Style::NORMAL;
		Font::RenderMode mode = Font::RenderMode::SOLID;
		uint16_t size = 0;
		Colour colour;
		std::shared_ptr<Image> image;

	private:
		void generateImage();
	};

	class GUIWidget
	{
	public:
		bool isClickable = false;
		bool isHighlightable = false;
		bool isAnchored = false;
		bool isPressed = false;
		bool isOverlayedToParent = false;
		glm::vec2 defaultDimensions;
		glm::vec2 position;
		std::string text;
		std::shared_ptr<Image> backgroundImage;
	};

	class GUIProgressBar
	{
	public:
		float progressValue = 0.0;
		float maximumValue = 0.0;
		glm::vec2 position;
	};

	class GUILine
	{
	public:
		glm::vec2 startPoint;
		glm::vec2 endPoint;
	};

	class GUIPanel
	{
	public:
		std::unordered_map<std::string, GUIWidget> widgets;
		std::unordered_map<std::string, GUIProgressBar> progressBars;
		std::unordered_map<std::string, GUILine> separators;
		float verticalSliderPosition = 0.0;
		float horizontalSliderPosition = 0.0;
	};

	class OMNIA_ENGINE_API GUI : public RenderableComponent
	{
		friend class GUISystem;
	public:
		GUI()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "GUI";
	private:
		std::unordered_map<std::string, GUIPanel> guiPanels;
	};
}