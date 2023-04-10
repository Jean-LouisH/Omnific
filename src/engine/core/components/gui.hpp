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

#include "core/assets/image.hpp"
#include "renderable_component.hpp"
#include "core/component.hpp"
#include "core/assets/colour.hpp"
#include <unordered_map>
#include <string>

namespace Omnia
{
	class OMNIA_ENGINE_API GUIText
	{
	public:
		GUIText()
		{
			this->image = std::shared_ptr<Image>(new Image());
		};

		void setText(std::string text);
		void setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px);
		void setFontSize(uint16_t size_px);
		void setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

		std::string text;
		std::shared_ptr<Omnia::Font> font;
		uint16_t size = 0;
		uint16_t wrapLength = 500;
		Colour colour;
		std::shared_ptr<Image> image;

	private:
		void generateImage();
	};

	class OMNIA_ENGINE_API GUIWidget
	{
	public:
		bool isClickable = false;
		bool isHighlightable = false;
		bool isAnchored = false;
		bool isPressed = false;
		bool isOverlayedToParent = false;
		glm::vec2 defaultDimensions;
		glm::vec2 position;
		GUIText guiText;
		std::shared_ptr<Image> backgroundImage;
	};

	class OMNIA_ENGINE_API GUIProgressBar
	{
	public:
		float progressValue = 0.0;
		float maximumValue = 0.0;
		glm::vec2 position;
	};

	class OMNIA_ENGINE_API GUILine
	{
	public:
		glm::vec2 startPoint;
		glm::vec2 endPoint;
	};

	class OMNIA_ENGINE_API GUIPanel
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

		std::unordered_map<std::string, GUIPanel> guiPanels;

		virtual Registerable* instance() override
		{
			GUI* clone = new GUI(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
	private:
		void updateImage();
	};
}