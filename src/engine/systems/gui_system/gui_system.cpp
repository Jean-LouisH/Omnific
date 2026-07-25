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

#include "gui_system.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include <foundations/singletons/event_bus.hpp>
#include <SDL3_ttf/SDL_ttf.h>
#include <scene/components/gui_element.hpp>
#include <scene/components/panel.hpp>
#include <scene/components/button.hpp>
#include <scene/components/button_list.hpp>
#include <foundations/transform.hpp>
#include <scene/components/viewport.hpp>
#include <scene/components/camera.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_manager.hpp>

#define GUI_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME "gui_system_on_early_update_frame_time"
#define GUI_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "gui_system_on_fixed_update_frame_time"

Omnific::GUISystem::~GUISystem()
{
	this->finalize();
}

void Omnific::GUISystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(GUI_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME, {"gui_system", "on_early_update_frame_time"});
	Profiler::add_clock(GUI_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"gui_system", "on_fixed_update_frame_time"});
	Platform::get_logger().write("Initialized GUI System.");
	if (!TTF_Init()) 
		SDL_Log("Couldn't initialize SDL_ttf: %s\n", SDL_GetError());
}

void Omnific::GUISystem::on_early_update()
{
	std::shared_ptr<Scene> scene = SceneManager::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(GUI_SYSTEM_ON_EARLY_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	Inputs& inputs = Platform::get_inputs();
	glm::vec2 mouse_position = inputs.get_mouse_position();
	
	/*Invert the y to increase from bottom to top.*/
	mouse_position.y = Platform::get_window().get_window_size().y - mouse_position.y;

	for (auto& renderable : scene->get_renderables_in_order())
	{
		if (renderable->is_gui_element())
		{
			std::shared_ptr<GUIElement> gui_element = std::dynamic_pointer_cast<GUIElement>(renderable);
			std::shared_ptr<Entity> gui_element_entity = scene->get_entity(gui_element->get_entity_id());
			glm::vec2 centre = gui_element->get_dimensions() * 0.5f;

			/* Informs GUIs if the mouse is hovering or clicking on them or their widgets. */
			if (gui_element->get_type() == Panel::TYPE_STRING)
			{

			}
			else if (gui_element->get_type() == ButtonList::TYPE_STRING || 
				gui_element->get_type() == ButtonList::TYPE_STRING)
			{
				// std::shared_ptr<ButtonList> gui_list = std::dynamic_pointer_cast<ButtonList>(gui_element);
				// std::shared_ptr<Entity> gui_list_entity = scene->get_entity(gui_list->get_entity_id());
				// std::vector<std::shared_ptr<Button>> gui_list_items =  gui_list->get_list_items();
				// this->detect_inputs_for_gui_element(
				// 	gui_element, 
				// 	glm::vec2(gui_list_entity->get_transform()->translation) - gui_list->get_position_pivot_offset(), 
				// 	mouse_position);

				// for (int i = 0; i < gui_list_items.size(); ++i)
				// {
				// 	std::shared_ptr<Button>& gui_list_item = gui_list_items[i];
				// 	this->detect_inputs_for_gui_element(
				// 		std::dynamic_pointer_cast<GUI::Element>(gui_list_item), 
				// 		gui_list_item->get_position() - gui_list_item->get_position_pivot_offset(),
				// 		mouse_position);
				// }
			}
			else
			{
				this->detect_inputs_for_gui_element(
					gui_element, 
					glm::vec2(gui_element_entity->get_transform()->translation) - gui_element->get_position_pivot_offset(),
					mouse_position);
			}
		}
	}
	frame_time_clock->set_end();
}

void Omnific::GUISystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneManager::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(GUI_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	Inputs& inputs = Platform::get_inputs();
	glm::vec2 mouse_position = inputs.get_mouse_position();
	
	/*Invert the y to increase from bottom to top.*/
	mouse_position.y = Platform::get_window().get_window_size().y - mouse_position.y;

	for (auto& renderable : scene->get_renderables_in_order())
	{
		if (renderable->is_gui_element())
		{
			std::shared_ptr<GUIElement> gui_element = std::dynamic_pointer_cast<GUIElement>(renderable);
			std::shared_ptr<Entity> gui_element_entity = scene->get_entity(gui_element->get_entity_id());
			std::shared_ptr<Transform> gui_transform = gui_element_entity->get_transform();

			glm::vec2 centre = gui_element->get_dimensions() * 0.5f;

			/* Enforce element pivot to some point on the GUI*/
			switch (gui_element->pivot)
			{
				case GUIElement::PivotPoint::TOP_LEFT: gui_element->position_pivot_offset = glm::vec2(-centre.x, centre.y); break;
				case GUIElement::PivotPoint::TOP_CENTRE: gui_element->position_pivot_offset = glm::vec2(0.0, centre.y); break;
				case GUIElement::PivotPoint::TOP_RIGHT: gui_element->position_pivot_offset = glm::vec2(centre.x, centre.y); break;
				case GUIElement::PivotPoint::CENTRE_LEFT: gui_element->position_pivot_offset = glm::vec2(-centre.x, 0.0); break;
				case GUIElement::PivotPoint::CENTRE: gui_element->position_pivot_offset = glm::vec2(0.0, 0.0); break;
				case GUIElement::PivotPoint::CENTRE_RIGHT: gui_element->position_pivot_offset = glm::vec2(centre.x, 0.0); break;
				case GUIElement::PivotPoint::BOTTOM_LEFT: gui_element->position_pivot_offset = glm::vec2(-centre.x, -centre.y); break;
				case GUIElement::PivotPoint::BOTTOM_CENTRE: gui_element->position_pivot_offset = glm::vec2(0.0, -centre.y); break;
				case GUIElement::PivotPoint::BOTTOM_RIGHT: gui_element->position_pivot_offset = glm::vec2(centre.x, -centre.y); break;
			}

			/* To enforce the GUI following a target Entity by an offset. */
			if (gui_element->is_following_entity)
			{
				std::shared_ptr<Entity> followed_entity = scene->get_entity_by_name(gui_element->follow_target_entity_name);

				if (followed_entity != nullptr)
				{
					std::shared_ptr<Transform> followed_entity_global_transform = scene->calculate_global_transform(followed_entity->get_id());
					std::vector<std::shared_ptr<Viewport>> ui_viewports = scene->get_components_by_type<Viewport>();
					std::shared_ptr<Camera> camera;
					std::shared_ptr<Transform> camera_global_transform;

					std::shared_ptr<Entity> viewport_entity = scene->get_entity_by_name(DEFAULT_VIEWPORT_NAME);
					std::shared_ptr<Viewport> viewport = scene->get_component_by_type<Viewport>(viewport_entity->get_id());
					std::shared_ptr<Entity> camera_entity = scene->get_entity_by_name(viewport->get_camera_entity_name());
					camera = scene->get_component_by_type<Camera>(camera_entity->get_id());
					camera_global_transform = scene->calculate_global_transform(camera_entity->get_id());

					if (camera != nullptr && camera_global_transform != nullptr)
					{
						/* Set the GUI position on an offset relative to the followed Entity in the Camera view. */

						if (followed_entity->is_2d)
						{
							float x = followed_entity_global_transform->translation.x - camera_global_transform->translation.x;
							float y = followed_entity_global_transform->translation.y - camera_global_transform->translation.y;
							float z_rotation = followed_entity_global_transform->get_rotation_in_euler_angles().z - camera_global_transform->get_rotation_in_euler_angles().z;

							gui_element_entity->get_transform()->translation = glm::vec3((gui_element->follow_offset + glm::vec2(
								x * cos(z_rotation) - y * sin(z_rotation), 
								x * sin(z_rotation) + y * cos(z_rotation)), 
								0.0
							));
						}
						else
						{
							glm::vec2 window_size = Platform::get_window().get_window_size();
							glm::mat4 world_to_view_matrix = glm::inverse(camera_global_transform->get_transform_matrix());
							glm::mat4 view_to_projection_matrix = camera->get_view_to_projection_matrix();
							glm::mat4 model_to_world_matrix = followed_entity_global_transform->get_transform_matrix();

							glm::vec4 clip_space = 
								view_to_projection_matrix * 
								world_to_view_matrix * 
								model_to_world_matrix * 
								glm::vec4(glm::vec3(0.0), 1.0);

							glm::vec3 ndc = glm::vec3(clip_space) / clip_space.w;

							gui_element_entity->get_transform()->translation = glm::vec3(gui_element->follow_offset + glm::vec2(
								(ndc.x + 1.0f) * 0.5f * window_size.x, 
								(ndc.y + 1.0f) * 0.5f * window_size.y
							), 0.0);

						}
					}
				}
			}

			glm::vec2 window_size = Platform::get_window().get_window_size();
			std::shared_ptr<Transform> gui_element_transform = gui_element_entity->get_transform();

			/* Enforce element anchorage to some point on the parent GUIElement*/
			switch (gui_element->anchor_pivot)
			{
				case GUIElement::PivotPoint::NONE:; break;
				case GUIElement::PivotPoint::TOP_LEFT: gui_element_transform->translation = glm::vec3(glm::vec2(0.0, window_size.y) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::TOP_CENTRE: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x / 2.0, window_size.y) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::TOP_RIGHT: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x, window_size.y) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::CENTRE_LEFT: gui_element_transform->translation = glm::vec3(glm::vec2(0.0, window_size.y / 2.0) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::CENTRE: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x / 2.0, window_size.y / 2.0) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::CENTRE_RIGHT: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x, window_size.y / 2.0) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::BOTTOM_LEFT: gui_element_transform->translation = glm::vec3(glm::vec2(0.0, 0.0) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::BOTTOM_CENTRE: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x / 2.0, 0.0) + gui_element->margin, 0.0); break;
				case GUIElement::PivotPoint::BOTTOM_RIGHT: gui_element_transform->translation = glm::vec3(glm::vec2(window_size.x, 0.0) + gui_element->margin, 0.0); break;
			}
		}
	}
	frame_time_clock->set_end();
}

void Omnific::GUISystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::GUISystem::detect_inputs_for_gui_element(
	std::shared_ptr<Omnific::GUIElement> gui_element, 
	glm::vec2 gui_element_global_position,
	glm::vec2 mouse_position)
{
	if (!gui_element->is_hidden())
	{
		Inputs& inputs = Platform::get_inputs();
		glm::vec2 gui_element_dimensions = gui_element->get_dimensions();
		float box_left = gui_element_global_position.x - gui_element_dimensions.x / 2.0;
		float box_right = gui_element_global_position.x + gui_element_dimensions.x / 2.0;
		float box_top = gui_element_global_position.y + gui_element_dimensions.y / 2.0;
		float box_bottom = gui_element_global_position.y - gui_element_dimensions.y / 2.0;

		bool is_mouse_hovering_over_gui_element = 
		((box_left <= mouse_position.x && box_right >= mouse_position.x) &&
		(box_bottom <= mouse_position.y && box_top >= mouse_position.y));

		bool is_any_mouse_button_pressed = 
			inputs.is_left_mouse_button_pressed() || 
			inputs.is_middle_mouse_button_pressed() || 
			inputs.is_right_mouse_button_pressed();

		bool is_any_mouse_button_on_release = 
			inputs.is_left_mouse_button_on_release() || 
			inputs.is_middle_mouse_button_on_release() || 
			inputs.is_right_mouse_button_on_release();


		if (glm::length(inputs.get_mouse_motion_velocity()) > 0 || 
			glm::length(inputs.get_mouse_wheel_velocity()) > 0 || 
			is_any_mouse_button_pressed || is_any_mouse_button_on_release)
		{
			// Only update when the mouse is hovering over the element and 
			// when none of the buttons are held pressed from a previous state 
			// of the gui not hovered in focus, and when any mouse input is detected. 
			if (is_mouse_hovering_over_gui_element && (!(!gui_element->is_hovered_in_focus) ||
				!(is_any_mouse_button_pressed)))
			{
				Inputs& inputs = Platform::get_inputs();
				gui_element->is_hovered_in_focus = true;

				if (inputs.is_left_mouse_button_on_release() &&
					gui_element->is_clicked && 
					gui_element->scene_hyperlink != "")
				{
					Platform::get_logger().write("GUISystem loading scene hyperlink: " + gui_element->scene_hyperlink);
					SceneManager::load_scene(std::shared_ptr<Scene>(new Scene(gui_element->scene_hyperlink)));
				}

				if (inputs.is_left_mouse_button_pressed())
				{
					gui_element->is_clicked = true;
				}
				else
				{
					gui_element->is_clicked = false;
				}
				gui_element->highlight_on_input();
			}
			else if (!is_mouse_hovering_over_gui_element && gui_element->is_hovered_in_focus)
			{
				gui_element->is_hovered_in_focus = false;
				gui_element->highlight_on_input();
			}
		}
	}
}