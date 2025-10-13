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

#include "foundations/resources/image.hpp"
#include <foundations/resources/default_assets/shaders/vertex_gui_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_gui_glsl.hpp>
#include "renderable.hpp"
#include "scene/components/component.hpp"
#include "foundations/colour.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <foundations/aliases.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API GUIElement
	{
		friend class GUI;
		friend class GUISystem;
		friend class GUIPanel;
		friend class GUIList;
	public:
		enum class GUIPoint
		{
			NONE,
			TOP_LEFT,
			TOP_CENTRE,
			TOP_RIGHT,
			CENTRE_LEFT,
			CENTRE,
			CENTRE_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_CENTRE,
			BOTTOM_RIGHT
		};

		static constexpr const char* TYPE_STRING = "GUIElement";
		GUIElement()
		{
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->gui_element_type = TYPE_STRING;
			this->image = std::shared_ptr<Image>(new Image());
			this->target_default_background_colour = std::shared_ptr<Colour>(new Colour(0.3, 0.3, 0.3, 1.0));
			this->target_highlight_colour = std::shared_ptr<Colour>(new Colour(0.5, 0.5, 0.5, 1.0));
			this->target_clicked_colour = std::shared_ptr<Colour>(new Colour(0.7, 0.7, 0.7, 1.0));
			this->target_current_colour = target_default_background_colour;
			this->dimensions = glm::vec2(0.0);
			this->position = glm::vec2(0.0);
			this->pivot = GUIPoint::TOP_LEFT;
			this->anchoring = GUIPoint::NONE;
			this->margin = glm::vec2(0.0);
		}
		/* The position has an increasing y that approaches the down direction. */
		bool is_hidden = false;
		bool is_clickable = false;
		bool is_highlightable = false;
		bool is_draggable = false;
		bool is_xstretched_to_panel = false;
		bool is_ystretched_to_panel = false;
		GUIPoint pivot;
		GUIPoint anchoring;

		virtual ~GUIElement() = default;
		void deserialize_common_properties(YAML::Node yaml_node);
		virtual void update_image();
		bool get_is_in_focus();
		bool get_is_selected();
		glm::vec2 get_position();
		glm::vec2 get_dimensions();
		glm::vec2 get_pivot_offset();
		std::string get_name();
		std::string get_gui_element_type();
		std::string get_parent_type();
		std::shared_ptr<Image> get_image();
	protected:
		bool is_hovered_in_focus = false;
		bool is_selected = false;

		glm::vec2 position;
		glm::vec2 dimensions;
		glm::vec2 pivot_offset;
		glm::vec2 margin;

		std::string name;
		std::string gui_element_type;
		std::string parent_type;
		std::shared_ptr<Image> image;
		std::shared_ptr<Colour> target_highlight_colour;
		std::shared_ptr<Colour> target_default_background_colour;
		std::shared_ptr<Colour> target_clicked_colour;
		std::shared_ptr<Colour> target_current_colour;

		void highlight_on_input();
	};

	class OMNIFIC_ENGINE_API GUIImage : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIImage";
		GUIImage()
		{
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->gui_element_type = TYPE_STRING;
			this->parent_type = GUIElement::TYPE_STRING;
		};

		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUIColour : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIColour";
		GUIColour()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		virtual void update_image() override;
	private:
		std::shared_ptr<Colour> colour;
	};

	class OMNIFIC_ENGINE_API GUILabel : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIText";
		GUILabel()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
			this->text_colour = std::shared_ptr<Colour>(new Colour(0.0, 0.0, 0.0, 1.0));
			this->font = std::shared_ptr<Font>(new Font());
		};
		void set_text(std::string text);
		void set_font(std::shared_ptr<Font> font);
		void set_text_colour(std::shared_ptr<Colour> colour);
		virtual void update_image() override;
	private:
		std::string text;
		std::shared_ptr<Font> font;
		uint16_t wrap_length = 500;
		std::shared_ptr<Colour> text_colour;
		bool is_edittable = false;
	};

	class OMNIFIC_ENGINE_API GUIButton : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
		friend class GUIList;
	public:
		static constexpr const char* TYPE_STRING = "GUIButton";
		GUIButton()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
			this->is_clickable = true;
			this->is_highlightable = true;
			this->gui_label = std::shared_ptr<GUILabel>(new GUILabel());
			this->button_space_from_text = glm::vec2(20.0);
		};
		virtual void update_image() override;
	protected:
		std::shared_ptr<GUILabel> gui_label;
		glm::vec2 button_space_from_text;
	};

	class OMNIFIC_ENGINE_API GUIToggleButton : public GUIButton
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIToggleButton";
		GUIToggleButton()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIButton::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUIList : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIListBox";
		GUIList()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		virtual void update_image() override;
		void add_item(std::string button_name);
		std::vector<std::shared_ptr<GUIButton>> get_list_items();
	protected:
		std::vector<std::shared_ptr<GUIButton>> list_items;
		uint64_t selected_item_index = 0;
	};

	class OMNIFIC_ENGINE_API GUITree : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUITree";
		GUITree()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		//virtual void update_image() override;
	protected:
		std::unordered_map<std::string, std::shared_ptr<GUIList>> gui_list_hierarchy;
	};

	class OMNIFIC_ENGINE_API GUIContextMenu : public GUITree
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIContextMenu";
		GUIContextMenu()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUITree::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUIMenuBar : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIMenuBar";
		GUIMenuBar()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
		std::unordered_map<std::string, GUIContextMenu> menus;
	};

	class OMNIFIC_ENGINE_API GUISpinner : public GUIList
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUISpinner";
		GUISpinner()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIList::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUITiles : public GUIList
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUITiles";
		GUITiles()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIList::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUIDropDownList : public GUIList
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIDropDownList";
		GUIDropDownList()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIList::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
	};

	class OMNIFIC_ENGINE_API GUISlider : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUISlider";
		GUISlider()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
		float current_value = 0.0;
		float minimum_value = 0.0;
		float maximum_value = 100.0;
		bool is_horizontal = true;
		bool is_vertical = false;
		bool is_progress_bar = false;
		bool is_displaying_spinner = false;
		std::string units_string;
		std::shared_ptr<GUISpinner> gui_spinner;
	};

	class OMNIFIC_ENGINE_API GUILine : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUILine";
		GUILine()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
		glm::vec2 start_point;
		glm::vec2 end_point;
	};

	class OMNIFIC_ENGINE_API GUITreeView : public GUITree
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUITreeView";
		GUITreeView()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUITree::TYPE_STRING;
		};
		//virtual void update_image() override;
	private:
		uint64_t current_item_index = 0;
	};

	class OMNIFIC_ENGINE_API GUIPanel : public GUIElement
	{
		friend class GUI;
		friend class GUISystem;
	public:
		static constexpr const char* TYPE_STRING = "GUIPanel";

		GUIPanel()
		{
			this->gui_element_type = TYPE_STRING;
			this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
			this->parent_type = GUIElement::TYPE_STRING;
			this->target_default_background_colour = std::shared_ptr<Colour>(new Colour(0.8, 0.8, 0.8, 1.0));
			this->target_scrollbar_colour = std::shared_ptr<Colour>(new Colour("#222222"));
			this->target_scrollbar_thumb_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
			this->target_scrollbar_arrow_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
			this->scrollbar_thickness = 5;
		}

		void add_gui_element(std::shared_ptr<GUIElement> gui_element);
		virtual void update_image() override;
	private:
		std::vector<std::shared_ptr<GUIElement>> gui_elements;
		std::shared_ptr<Colour> target_scrollbar_colour;
		std::shared_ptr<Colour> target_scrollbar_thumb_colour;
		std::shared_ptr<Colour> target_scrollbar_arrow_colour;
		uint32_t scrollbar_thickness;

		/* Denoted in the image pixel position of the top view of the panel
		   where an increasing y value travels from top to bottom. */
		uint32_t vertical_slider_position = 0;
		uint32_t horizontal_slider_position = 0;

		std::shared_ptr<Image> scrollbar_arrow_image;
		std::shared_ptr<Image> scrollbar_image;
		std::shared_ptr<Image> vertical_scrollbar_thumb_image;
		std::shared_ptr<Image> horizontal_scrollbar_thumb_image;
	};

	class OMNIFIC_ENGINE_API GUI : public Renderable
	{
		friend class GUISystem;
	public:
		GUI()
		{
			this->type = TYPE_STRING;
			this->shader = std::shared_ptr<Shader>(new Shader(DefaultAssets::vertex_gui_glsl, DefaultAssets::fragment_gui_glsl, false, false));
		};
		static constexpr const char* TYPE_STRING = "GUI";

		virtual Registerable* instance() override
		{
			GUI* clone = new GUI(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		bool is_2d_override = true;
		glm::vec2 follow_offset;

		virtual void deserialize(YAML::Node yaml_node);
		void set_to_label(std::string text);

		std::shared_ptr<GUIElement> get_element(std::string gui_element_name);
		std::shared_ptr<GUIElement> get_root_element();
		void update_image();
	private:
		std::unordered_map<std::string, std::shared_ptr<GUIElement>> element_cache;
		std::shared_ptr<GUIElement> root_element;

		std::string follow_target_entity_name;
		bool is_following_entity = false;

		std::shared_ptr<GUIElement> deserialize_gui_element(YAML::Node yaml_node);
	};
}