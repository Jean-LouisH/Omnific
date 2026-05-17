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

#include <foundations/aliases.hpp>
#include <foundations/constants.hpp>
#include "scene/components/component.hpp"
#include "scene/components/renderable.hpp"
#include <foundations/resources/default_assets/shaders/vertex_gui_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_gui_glsl.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API GUIElement : public Renderable
	{
		friend class GUISystem;
	public:
		GUIElement()
		{
			this->type = TYPE_STRING;
			this->set_to_image(std::shared_ptr<Image>(new Image()));
			this->shader = std::shared_ptr<Shader>(new Shader(DefaultAssets::vertex_gui_glsl, DefaultAssets::fragment_gui_glsl, false, false));
			this->target_default_background_colour = std::shared_ptr<Colour>(new Colour(0.3, 0.3, 0.3, 1.0));
			this->target_highlight_colour = std::shared_ptr<Colour>(new Colour(0.5, 0.5, 0.5, 1.0));
			this->target_clicked_colour = std::shared_ptr<Colour>(new Colour(0.7, 0.7, 0.7, 1.0));
			this->target_current_colour = target_default_background_colour;
			this->dimensions = glm::vec3(0.0);
			this->pivot = PivotPoint::TOP_LEFT;
			this->anchor_pivot = PivotPoint::NONE;
			this->margin = glm::vec2(0.0);
		};

		virtual ~GUIElement() = default;

		static constexpr const char* TYPE_STRING = "GUIElement";

		virtual Registerable* instance() override
		{
			GUIElement* clone = new GUIElement(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		virtual void deserialize(YAML::Node yaml_node);

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
		
		bool is_clickable = false;
		bool is_highlightable = false;
		bool is_draggable = false;
		bool is_xstretched_to_panel = false;
		bool is_ystretched_to_panel = false;
		PivotPoint pivot;
		PivotPoint anchor_pivot;

		bool is_hovered_in_focus = false;
		bool is_selected = false;
		bool is_clicked = false;

		glm::vec2 position_pivot_offset = glm::vec2(0.0);
		glm::vec2 margin = glm::vec2(0.0);

		std::shared_ptr<Colour> target_highlight_colour;
		std::shared_ptr<Colour> target_default_background_colour;
		std::shared_ptr<Colour> target_clicked_colour;
		std::shared_ptr<Colour> target_current_colour;

		std::string follow_target_entity_name;
		bool is_following_entity = false;
		glm::vec2 follow_offset = glm::vec2(0.0);
		bool is_2d_override = true;

		virtual void update_image();
		bool get_is_in_focus();
		bool get_is_selected();
		glm::vec2 get_position_pivot_offset();
	protected:
		void highlight_on_input();
	private:
	};
}