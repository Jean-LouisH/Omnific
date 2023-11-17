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
	class OMNIA_ENGINE_API GUIElement
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIElement";
		GUIElement()
		{
			this->name = "GUIElement ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->gui_element_type = TYPE_STRING;
			this->image = std::shared_ptr<Image>(new Image());
			this->background_colour = std::shared_ptr<Colour>(new Colour(0.3, 0.3, 0.3, 1.0));
			this->dimensions = glm::vec2(0.0);
			this->position = glm::vec2(0.0);
		}

		virtual ~GUIElement() = default;

		struct DetectedInputs
		{
			bool is_hovered = false;
			bool is_left_mouse_button_on_press = false;
			bool is_left_mouse_button_on_release = false;
			bool is_left_mouse_button_double_clicked = false;
			bool is_middle_mouse_button_on_press = false;
			bool is_middle_mouse_button_on_release = false;
			bool is_middle_mouse_button_double_clicked = false;
			bool is_right_mouse_button_on_press = false;
			bool is_right_mouse_button_on_release = false;
			bool is_right_mouse_button_double_clicked = false;

		} detected_inputs;

		std::string name;
		std::string gui_element_type;

		/* The position has an increasing y that approaches the down direction. */
		glm::vec2 position;
		glm::vec2 dimensions;
		std::shared_ptr<Image> image;
		std::shared_ptr<Colour> background_colour;
	};

	class OMNIA_ENGINE_API GUIWidget : public GUIElement
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIWidget";
		GUIWidget()
		{
			this->background_colour = std::shared_ptr<Colour>(new Colour(0.5, 0.5, 0.5, 1.0));
			this->gui_element_type = TYPE_STRING;
		}

		virtual ~GUIWidget() = default;

		bool is_clickable = false;
		bool is_highlightable = false;
		bool is_anchored = false;
		bool is_xstretched_to_panel = false;
		bool is_ystretched_to_panel = false;
	};

	class OMNIA_ENGINE_API GUIImage : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIImage";
		GUIImage()
		{
			this->gui_element_type = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIColour : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIColour";
		GUIColour()
		{
			this->gui_element_type = TYPE_STRING;
		};

		std::shared_ptr<Colour> colour;

		void update_image();
	};

	class OMNIA_ENGINE_API GUIText : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIText";
		GUIText()
		{
			this->gui_element_type = TYPE_STRING;
		};

		void set_text(std::string text);
		void set_font(std::shared_ptr<Omnia::Font> font, uint16_t size_px);
		void set_font_size(uint16_t size_px);
		void set_colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

		std::string text;
		std::shared_ptr<Omnia::Font> font;
		uint16_t size = 0;
		uint16_t wrap_length = 500;
		std::shared_ptr<Colour> colour;
		bool is_edittable = false;

		void update_image();
	};

	class OMNIA_ENGINE_API GUIButton : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIButton";
		GUIButton()
		{
			this->gui_element_type = TYPE_STRING;
			this->gui_text = std::shared_ptr<GUIText>(new GUIText());
			this->button_space_from_text = glm::vec2(5.0);
		};

		std::shared_ptr<GUIText> gui_text;
		glm::vec2 button_space_from_text;

		void update_image();

	};

	class OMNIA_ENGINE_API GUIToggleButton : public GUIButton
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIToggleButton";
		GUIToggleButton()
		{
			this->gui_element_type = TYPE_STRING;
		};

	};

	class OMNIA_ENGINE_API GUIList : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIListBox";
		GUIList()
		{
			this->gui_element_type = TYPE_STRING;
		};

		std::vector<GUIButton> list_items;
	};

	class OMNIA_ENGINE_API GUITree : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITree";
		GUITree()
		{
			this->gui_element_type = TYPE_STRING;
		};

		std::unordered_map<std::string, GUIButton> items;
		std::unordered_map<std::string, std::string> item_hierarchy;
	};

	class OMNIA_ENGINE_API GUIContextMenu : public GUITree
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIContextMenu";
		GUIContextMenu()
		{
			this->gui_element_type = TYPE_STRING;
		};

	};

	class OMNIA_ENGINE_API GUIMenuBar : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIMenuBar";
		GUIMenuBar()
		{
			this->gui_element_type = TYPE_STRING;
		};

		std::unordered_map<std::string, GUIContextMenu> menus;

	};

	class OMNIA_ENGINE_API GUISpinner : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUISpinner";
		GUISpinner()
		{
			this->gui_element_type = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUITiles : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITiles";
		GUITiles()
		{
			this->gui_element_type = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIDropDownList : public GUIList
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIDropDownList";
		GUIDropDownList()
		{
			this->gui_element_type = TYPE_STRING;
		};

		uint64_t current_item_index = 0;
	};

	class OMNIA_ENGINE_API GUISlider : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUISlider";
		GUISlider()
		{
			this->gui_element_type = TYPE_STRING;
		};
		float current_value = 0.0;
		float minimum_value = 0.0;
		float maximum_value = 100.0;
		bool is_horizontal = true;
		bool is_vertical = false;
		bool is_progres_bar = false;
		bool is_displaying_spinner = false;
		std::string units_string;
		GUISpinner gui_spinner;
	};

	class OMNIA_ENGINE_API GUILine : public GUIWidget
	{
	public:
		static constexpr const char* TYPE_STRING = "GUILine";
		GUILine()
		{
			this->gui_element_type = TYPE_STRING;
		};

		glm::vec2 start_point;
		glm::vec2 end_point;
	};

	class OMNIA_ENGINE_API GUITreeView : public GUITree
	{
	public:
		static constexpr const char* TYPE_STRING = "GUITreeView";
		GUITreeView()
		{
			this->gui_element_type = TYPE_STRING;
		};
	};

	class OMNIA_ENGINE_API GUIPanel : public GUIElement
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIPanel";

		GUIPanel()
		{
			this->gui_element_type = TYPE_STRING;
		}

		std::string name;
		std::unordered_map<UID, std::shared_ptr<GUIWidget>> widgets;
		std::shared_ptr<Colour> scrollbar_colour;
		std::shared_ptr<Colour> scrollbar_thumb_colour;
		std::shared_ptr<Colour> scrollbar_arrow_colour;
		std::shared_ptr<Image> scrollbar_arrow_image;
		std::shared_ptr<Image> scrollbar_scrollbar_image;
		std::shared_ptr<Image> vertical_scrollbar_thumb_image;
		std::shared_ptr<Image> horizontal_scrollbar_thumb_image;

		/* Denoted in the image pixel position of the top view of the panel
		   where an increasing y value travels from top to bottom. */
		uint32_t vertical_slider_position = 0;
		uint32_t horizontal_slider_position = 0;

		void update_image();
	};

	class OMNIA_ENGINE_API GUIPanelTabGroup : public GUIElement
	{
	public:
		static constexpr const char* TYPE_STRING = "GUIPanelTabGroup";

		GUIPanelTabGroup()
		{
			this->gui_element_type = TYPE_STRING;
		}

		std::string name;
		std::unordered_map<std::string, std::shared_ptr<GUIPanel>> gui_panels;
		std::string active_gui_panel_name;
		std::shared_ptr<Colour> highlighted_tab_colour;

		void update_image();
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
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::string follow_target_entity_name;
		glm::vec2 follow_offset;
		bool is_following_entity = false;

		virtual void deserialize(YAML::Node yaml_node);
		void set_as_text(std::string text);
		std::shared_ptr<GUIWidget> get_widget(std::string widget_name);
	private:
		std::unordered_map<std::string, std::shared_ptr<GUIPanelTabGroup>> gui_panel_tab_groups;
		std::unordered_map<std::string, std::shared_ptr<GUIWidget>> gui_widget_registry;

		void update_image();
	};
}