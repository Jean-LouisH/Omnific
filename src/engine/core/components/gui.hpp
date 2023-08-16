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
#include <vector>
#include <string>
#include <core/utilities/aliases.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API GUIWidget
	{
	public:
		GUIWidget()
		{
			this->image = std::shared_ptr<Image>(new Image());
			this->id = UIDGenerator::getNewUID();
		}

		virtual ~GUIWidget() = default;

		UID id;
		std::string widgetType;
		bool isClickable = false;
		bool isHighlightable = false;
		bool isAnchored = false;

		struct DetectedInputs
		{
			bool isHovered = false;
			bool isLeftMouseButtonOnPress = false;
			bool isLeftMouseButtonOnRelease = false;
			bool isLeftMouseButtonDoubleClicked = false;
			bool isMiddleMouseButtonOnPress = false;
			bool isMiddleMouseButtonOnRelease = false;
			bool isMiddleMouseButtonDoubleClicked = false;
			bool isRightMouseButtonOnPress = false;
			bool isRightMouseButtonOnRelease = false;
			bool isRightMouseButtonDoubleClicked = false;
		} detectedInputs;

		bool isXStretchedToPanel = false;
		bool isYStretchedToPanel = false;
		glm::vec2 defaultDimensions;
		glm::vec2 position;
		std::shared_ptr<Image> image;
	};

	class OMNIA_ENGINE_API GUIImage : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIImage";
		GUIImage()
		{
			this->widgetType = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIColour : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIColour";
		GUIColour()
		{
			this->widgetType = TYPE_STRING;
		};

		std::shared_ptr<Colour> colour;
	};

	class OMNIA_ENGINE_API GUIText : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIText";
		GUIText()
		{
			this->widgetType = TYPE_STRING;
		};

		void setText(std::string text);
		void setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px);
		void setFontSize(uint16_t size_px);
		void setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

		std::string text;
		std::shared_ptr<Omnia::Font> font;
		uint16_t size = 0;
		uint16_t wrapLength = 500;
		std::shared_ptr<Colour> colour;
		bool isEdittable = false;

		void generateImage();
	};

	class OMNIA_ENGINE_API GUIButton : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIButton";
		GUIButton()
		{
			this->widgetType = TYPE_STRING;
		};

		GUIText guiText;

	};

	class OMNIA_ENGINE_API GUIToggleButton : public GUIButton
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIToggleButton";
		GUIToggleButton()
		{
			this->widgetType = TYPE_STRING;
		};

	};

	class OMNIA_ENGINE_API GUIList : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIListBox";
		GUIList()
		{
			this->widgetType = TYPE_STRING;
		};

		std::vector<GUIButton> listItems;
	};

	class OMNIA_ENGINE_API GUITree : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITree";
		GUITree()
		{
			this->widgetType = TYPE_STRING;
		};

		std::unordered_map<std::string, GUIButton> items;
		std::unordered_map<std::string, std::string> itemHierarchy;
	};

	class OMNIA_ENGINE_API GUIContextMenu : public GUITree
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIContextMenu";
		GUIContextMenu()
		{
			this->widgetType = TYPE_STRING;
		};

	};

	class OMNIA_ENGINE_API GUIMenuBar : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIMenuBar";
		GUIMenuBar()
		{
			this->widgetType = TYPE_STRING;
		};

		std::unordered_map<std::string, GUIContextMenu> menus;

	};

	class OMNIA_ENGINE_API GUISpinner : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUISpinner";
		GUISpinner()
		{
			this->widgetType = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUITiles : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITiles";
		GUITiles()
		{
			this->widgetType = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIDropDownList : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIDropDownList";
		GUIDropDownList()
		{
			this->widgetType = TYPE_STRING;
		};

		uint64_t currentItemIndex = 0;
	};

	class OMNIA_ENGINE_API GUISlider : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUISlider";
		GUISlider()
		{
			this->widgetType = TYPE_STRING;
		};
		float currentValue = 0.0;
		float minimumValue = 0.0;
		float maximumValue = 100.0;
		bool isHorizontal = true;
		bool isVertical = false;
		bool isProgresBar = false;
		bool isDisplayingSpinner = false;
		std::string unitsString;
		GUISpinner guiSpinner;
	};

	class OMNIA_ENGINE_API GUILine : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUILine";
		GUILine()
		{
			this->widgetType = TYPE_STRING;
		};

		glm::vec2 startPoint;
		glm::vec2 endPoint;
	};

	class OMNIA_ENGINE_API GUITreeView : public GUITree
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITreeView";
		GUITreeView()
		{
			this->widgetType = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIPanel
	{
	public:
		std::string name;
		std::unordered_map<UID, std::shared_ptr<GUIWidget>> widgets;

		/* Denoted in the image pixel position of the top view of the panel
		   where an increasing y value travels from top to bottom. */
		float verticalSliderPosition = 0.0;
		float horizontalSliderPosition = 0.0;
	};

	class OMNIA_ENGINE_API GUIPanelTabGroup
	{
	public:
		std::string name;
		glm::vec2 position;
		glm::vec2 dimensions;
		std::unordered_map<std::string, std::shared_ptr<GUIPanel>> guiPanels;
		std::string activeGuiPanelName;
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

		virtual Registerable* instance() override
		{
			GUI* clone = new GUI(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}

		std::string followTargetEntityName;
		glm::vec2 followOffset;
		bool isFollowingEntity = false;

		virtual void deserialize(YAML::Node yamlNode);
		void setAsText(std::string text);
	private:
		/* In pixels. */
		glm::vec2 position;
		std::unordered_map<std::string, std::shared_ptr<GUIPanelTabGroup>> guiPanelTabGroups;

		void updateImage();
	};
}