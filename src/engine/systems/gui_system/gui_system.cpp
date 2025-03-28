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
#include <SDL_ttf.h>
#include <scene/components/gui.hpp>
#include <foundations/transform.hpp>
#include <scene/components/viewport.hpp>
#include <scene/components/camera.hpp>

Omnific::GUISystem::~GUISystem()
{
	this->finalize();
}

void Omnific::GUISystem::initialize()
{
	this->is_initialized = true;
	Platform::get_logger().write("Initialized GUI System.");
	if (TTF_Init() == -1)
		printf("TTF_Init: %s\n", TTF_GetError());
}

void Omnific::GUISystem::on_early_update(std::shared_ptr<Scene> scene)
{
	Inputs& inputs = Platform::get_inputs();
	std::unordered_map<std::string, double> numbers;
	std::unordered_map<std::string, std::string> strings;

	/* Sends an Engine loop event for a detected file drop. */
	if (inputs.is_drop_file_detected())
	{
		numbers.emplace((std::string)"drop_file_window_id", (double)inputs.get_drop_file_window_id());
		strings.emplace((std::string)"drop_file_path", inputs.get_drop_file_path());
		EventBus::publish("file dropped on window", strings, numbers);
	}

	glm::vec2 mouse_position = inputs.get_mouse_position();
	
	/*Invert the y to increase from bottom to top.*/
	mouse_position.y = Platform::get_window().get_window_size().y - mouse_position.y;

	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::vector<std::shared_ptr<GUI>> guis = scene_layer_it.second->get_components_by_type<GUI>();

		for (int i = 0; i < guis.size(); i++)
		{
			std::shared_ptr<GUI> gui = guis[i];
			std::shared_ptr<Transform> gui_transform = scene_layer_it.second->get_entity(gui->get_entity_id())->get_transform();

			/* To enforce the GUI following a target Entity by an offset. */
			if (gui->is_following_entity)
			{
				std::shared_ptr<Entity> followed_entity = scene_layer_it.second->get_entity_by_name(gui->follow_target_entity_name);
				
				/* If the Entity is not in the current SceneLayer, check 
				   through every other SceneLayer in the Scene. */
				if (followed_entity == nullptr)
				{
					for (auto scene_layer : scene->get_scene_layers())
					{
						followed_entity = scene_layer.second->get_entity_by_name(gui->follow_target_entity_name);
						if (followed_entity != nullptr)
						{
							break;
						}
					}
				}

				if (followed_entity != nullptr)
				{
					std::shared_ptr<Transform> followed_entity_transform = followed_entity->get_transform();
					std::vector<std::shared_ptr<Viewport>> ui_viewports = scene_layer_it.second->get_components_by_type<Viewport>();
					std::shared_ptr<Camera> camera;
					std::shared_ptr<Transform> camera_transform;

					for (int i = 0; i < ui_viewports.size(); i++)
					{
						std::shared_ptr<Viewport> ui_viewport = ui_viewports[i];
						std::shared_ptr<Entity> camera_entity = scene_layer_it.second->get_entity_by_name(ui_viewport->get_camera_entity_name());
						camera = scene_layer_it.second->get_component_by_type<Camera>(camera_entity->get_id());
						camera_transform = camera_entity->get_transform();
					}

					if (camera != nullptr && camera_transform != nullptr)
					{
						/* Set the GUI position on an offset relative to the followed Entity in the Camera view. */

						if (scene_layer_it.second->is_2d)
						{

						}
						else
						{

						}
					}
				}
			}

			glm::vec2 gui_position = gui_transform->translation;
			std::shared_ptr<GUIElement> root_element = gui->get_root_element();

			/* The root_element_dimensions is the boundary of the mouse position search
			   regardless of the root_element being a GUIPanel or not. */
			glm::vec2 root_element_dimensions = root_element->dimensions;

			/* Informs GUIs if the mouse is hovering or clicking on them or their widgets. */
			if (root_element->get_gui_element_type() == GUIPanel::TYPE_STRING)
			{

			}
			else if (root_element->get_gui_element_type() == GUIList::TYPE_STRING || 
				root_element->get_parent_type() == GUIList::TYPE_STRING)
			{
				std::shared_ptr<GUIList> gui_list = std::dynamic_pointer_cast<GUIList>(root_element);
				std::vector<std::shared_ptr<GUIButton>> gui_list_items =  gui_list->get_list_items();
				this->detect_inputs_for_gui_element(root_element, gui_position, mouse_position);
				for (int i = 0; i < gui_list_items.size(); i++)
				{
					this->detect_inputs_for_gui_element(std::dynamic_pointer_cast<GUIElement>(gui_list_items.at(i)), gui_position, mouse_position);
				}
			}
			else
			{
				this->detect_inputs_for_gui_element(root_element, gui_position, mouse_position);
			}

			gui->update_image();
		}
	}
}

void Omnific::GUISystem::finalize()
{
	this->is_initialized = false;
}

void Omnific::GUISystem::detect_inputs_for_gui_element(
	std::shared_ptr<Omnific::GUIElement> gui_element, 
	glm::vec2 gui_position,
	glm::vec2 mouse_position)
{
	if (!gui_element->is_hidden)
	{
		Inputs& inputs = Platform::get_inputs();
		glm::vec2 gui_element_global_position = gui_position + gui_element->position;

		glm::vec2 gui_element_dimensions = gui_element->get_image()->get_dimensions();

		float box_left = gui_element_global_position.x - gui_element_dimensions.x / 2.0;
		float box_right = gui_element_global_position.x + gui_element_dimensions.x / 2.0;
		float box_top = gui_element_global_position.y + gui_element_dimensions.y / 2.0;
		float box_bottom = gui_element_global_position.y - gui_element_dimensions.y / 2.0;

		bool is_mouse_hovering_over_gui_element = 
		((box_left <= mouse_position.x && box_right >= mouse_position.x) &&
		(box_bottom <= mouse_position.y && box_top >= mouse_position.y));

		if (is_mouse_hovering_over_gui_element && (!(!gui_element->is_hovered_in_focus) ||
			!(inputs.is_left_mouse_button_pressed() || 
			inputs.is_middle_mouse_button_pressed() || 
			inputs.is_right_mouse_button_pressed())))
		{
			gui_element->is_hovered_in_focus = true;
		}
		else if (!is_mouse_hovering_over_gui_element)
		{
			gui_element->is_hovered_in_focus = false;
		}
		
		//gui_element->update_image();
	}
}