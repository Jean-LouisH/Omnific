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
	class OMNIFIC_ENGINE_API GUI : public Renderable
	{
		friend class GUISystem;
	public:
		class OMNIFIC_ENGINE_API Element
		{
			friend class GUI;
			friend class GUISystem;
			friend class GUIPanel;
			friend class ButtonList;
		public:
			enum class PivotPoint
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
			
			static constexpr const char* TYPE_STRING = "Element";
			Element()
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
				this->pivot = PivotPoint::TOP_LEFT;
				this->anchor_pivot = PivotPoint::NONE;
				this->margin = glm::vec2(0.0);
			}
			/* The position has an increasing y that approaches the down direction. */
			bool is_hidden = false;
			bool is_clickable = false;
			bool is_highlightable = false;
			bool is_draggable = false;
			bool is_xstretched_to_panel = false;
			bool is_ystretched_to_panel = false;
			PivotPoint pivot;
			PivotPoint anchor_pivot;

			virtual ~Element() = default;
			void deserialize_common_properties(YAML::Node yaml_node);
			virtual void update_image();
			bool get_is_in_focus();
			bool get_is_selected();
			glm::vec2 get_position();
			glm::vec2 get_dimensions();
			glm::vec2 get_position_pivot_offset();
			std::string get_name();
			std::string get_gui_element_type();
			std::string get_parent_type();
			std::shared_ptr<Image> get_image();

			bool is_hovered_in_focus = false;
			bool is_selected = false;

			glm::vec2 position = glm::vec2(0.0);
			glm::vec2 dimensions = glm::vec2(0.0);
			glm::vec2 position_pivot_offset = glm::vec2(0.0);
			glm::vec2 margin = glm::vec2(0.0);

			std::string name;
			std::string gui_element_type;
			std::string parent_type;
			std::shared_ptr<Image> image;
			std::shared_ptr<Colour> target_highlight_colour;
			std::shared_ptr<Colour> target_default_background_colour;
			std::shared_ptr<Colour> target_clicked_colour;
			std::shared_ptr<Colour> target_current_colour;
		protected:
			void highlight_on_input();
		};

		class OMNIFIC_ENGINE_API ImageCanvas : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ImageCanvas";
			ImageCanvas()
			{
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->gui_element_type = TYPE_STRING;
				this->parent_type = Element::TYPE_STRING;
			};

			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API PlotCanvas : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "PlotCanvas";
			PlotCanvas()
			{
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->gui_element_type = TYPE_STRING;
				this->parent_type = Element::TYPE_STRING;
			};

			std::vector<float> plot_points;
			std::shared_ptr<Colour> target_plot_background_colour;
			std::shared_ptr<Colour> target_plot_colour;

			virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API ColourCanvas : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ColourCanvas";
			ColourCanvas()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			std::shared_ptr<Colour> colour;

			virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API Label : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "Label";
			Label()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
				this->text_colour = std::shared_ptr<Colour>(new Colour(0.0, 0.0, 0.0, 1.0));
				this->font = std::shared_ptr<Font>(new Font());
			};

			std::string text;
			std::shared_ptr<Font> font;
			uint16_t wrap_length = 500;
			std::shared_ptr<Colour> text_colour;
			bool is_edittable = false;

			void set_text(std::string text);
			void set_font(std::shared_ptr<Font> font);
			void set_text_colour(std::shared_ptr<Colour> colour);
			virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API Button : public Element
		{
			friend class GUI;
			friend class GUISystem;
			friend class ButtonList;
		public:
			static constexpr const char* TYPE_STRING = "Button";
			Button()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
				this->is_clickable = true;
				this->is_highlightable = true;
				this->gui_label = std::shared_ptr<Label>(new Label());
				this->button_space_from_text = glm::vec2(20.0);
			};

			std::shared_ptr<Label> gui_label;
			glm::vec2 button_space_from_text = glm::vec2(0.0);

			virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API ToggleButton : public Button
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ToggleButton";
			ToggleButton()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Button::TYPE_STRING;
			};
			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API ButtonList : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ButtonList";
			ButtonList()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			std::vector<std::shared_ptr<Button>> list_items;
			uint64_t selected_item_index = 0;

			virtual void update_image() override;
			void add_item(std::string button_name);
			std::vector<std::shared_ptr<Button>> get_list_items();
		};

		class OMNIFIC_ENGINE_API ButtonTree : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ButtonTree";
			ButtonTree()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			std::unordered_map<std::string, std::shared_ptr<ButtonList>> gui_list_hierarchy;

			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API ContextMenu : public ButtonTree
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ContextMenu";
			ContextMenu()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = ButtonTree::TYPE_STRING;
			};
			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API MenuBar : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "MenuBar";
			MenuBar()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			std::unordered_map<std::string, ContextMenu> menus;

			//virtual void update_image() override;			
		};

		class OMNIFIC_ENGINE_API Spinner : public ButtonList
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "Spinner";
			Spinner()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = ButtonList::TYPE_STRING;
			};
			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API Tiles : public ButtonList
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "Tiles";
			Tiles()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = ButtonList::TYPE_STRING;
			};
			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API DropDownButtonList : public ButtonList
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "DropDownButtonList";
			DropDownButtonList()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = ButtonList::TYPE_STRING;
			};
			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API Slider : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "Slider";
			Slider()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			float current_value = 0.0;
			float minimum_value = 0.0;
			float maximum_value = 100.0;
			bool is_horizontal = true;
			bool is_vertical = false;
			bool is_progress_bar = false;
			bool is_displaying_spinner = false;
			std::string units_string;
			std::shared_ptr<Spinner> gui_spinner;

			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API LineElement : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "LineElement";
			LineElement()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
			};

			glm::vec2 start_point = glm::vec2(0.0);
			glm::vec2 end_point = glm::vec2(0.0);

			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API ButtonTreeView : public ButtonTree
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "ButtonTreeView";
			ButtonTreeView()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = ButtonTree::TYPE_STRING;
			};

			uint64_t current_item_index = 0;

			//virtual void update_image() override;
		};

		class OMNIFIC_ENGINE_API Panel : public Element
		{
			friend class GUI;
			friend class GUISystem;
		public:
			static constexpr const char* TYPE_STRING = "Panel";

			Panel()
			{
				this->gui_element_type = TYPE_STRING;
				this->name = (std::string)TYPE_STRING + " ID: " + std::to_string(UIDGenerator::get_new_uid());
				this->parent_type = Element::TYPE_STRING;
				this->target_default_background_colour = std::shared_ptr<Colour>(new Colour(0.8, 0.8, 0.8, 1.0));
				this->target_scrollbar_colour = std::shared_ptr<Colour>(new Colour("#222222"));
				this->target_scrollbar_thumb_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
				this->target_scrollbar_arrow_colour = std::shared_ptr<Colour>(new Colour("#3b3b3b"));
				this->scrollbar_thickness = 5;
			}

			std::vector<std::shared_ptr<Element>> gui_elements;
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

			void add_gui_element(std::shared_ptr<Element> gui_element);
			virtual void update_image() override;
		};

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
		glm::vec2 follow_offset = glm::vec2(0.0);

		virtual void deserialize(YAML::Node yaml_node);
		void set_to_label(std::string text);
		void set_to_plot(std::vector<float> plot_points, std::shared_ptr<Colour> background_colour, std::shared_ptr<Colour> plot_colour);
		void set_to_colour_canvas(std::shared_ptr<Colour> colour);
		void set_to_image_canvas(std::shared_ptr<Image> image);

		std::shared_ptr<Element> get_element(std::string gui_element_name);
		std::shared_ptr<Element> get_root_element();
		void update_image();
	private:
		std::unordered_map<std::string, std::shared_ptr<Element>> element_cache;
		std::shared_ptr<Element> root_element;

		std::string follow_target_entity_name;
		bool is_following_entity = false;

		std::shared_ptr<Element> deserialize_gui_element(YAML::Node yaml_node);
	};
}