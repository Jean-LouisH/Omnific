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
#include <scene/components/transform.hpp>
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

void Omnific::GUISystem::on_late(std::shared_ptr<Scene> scene)
{
	Input& input = Platform::get_input();
	std::unordered_map<std::string, double> numbers;
	std::unordered_map<std::string, std::string> strings;

	/* Sends an Engine loop event for a detected file drop. */
	if (input.is_drop_file_detected())
	{
		numbers.emplace((std::string)"drop_file_window_id", (double)input.get_drop_file_window_id());
		strings.emplace((std::string)"drop_file_path", input.get_drop_file_path());
		EventBus::publish("file dropped on window", numbers, strings);
	}

	glm::vec2 mouse_position = input.get_mouse_position();
	mouse_position.y = Platform::get_window().get_window_size().y - mouse_position.y;

	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::vector<std::shared_ptr<GUI>> guis = scene_layer_it.second->get_components_by_type<GUI>();

		for (int i = 0; i < guis.size(); i++)
		{
			std::shared_ptr<GUI> gui = guis[i];
			std::shared_ptr<Transform> gui_transform = scene_layer_it.second->get_component_by_type<Transform>(gui->get_entity_id());
			float mouse_detection_accuracy_range = 20.0;

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
					std::shared_ptr<Transform> followed_entity_transform = scene_layer_it.second->get_component_by_type<Transform>(followed_entity->get_id());
					std::vector<std::shared_ptr<Viewport>> ui_viewports = scene_layer_it.second->get_components_by_type<Viewport>();
					std::shared_ptr<Camera> camera;
					std::shared_ptr<Transform> camera_transform;

					for (int i = 0; i < ui_viewports.size(); i++)
					{
						std::shared_ptr<Viewport> ui_viewport = ui_viewports[i];
						std::shared_ptr<Entity> camera_entity = scene_layer_it.second->get_entity_by_name(ui_viewport->get_camera_entity_name());
						camera = scene_layer_it.second->get_component_by_type<Camera>(camera_entity->get_id());
						camera_transform = scene_layer_it.second->get_component_by_type<Transform>(camera_entity->get_id());
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
			if (root_element->get_gui_element_type() != GUIPanel::TYPE_STRING)
			{
				std::shared_ptr<GUIElement> gui_element = root_element;
				glm::vec2 gui_element_global_position = gui_position + root_element->position;
				gui_element->detected_inputs = { 0 };

				/* If the mouse is at least hovering over the GUI widget. */
				if (glm::length(mouse_position - gui_element_global_position) < mouse_detection_accuracy_range)
				{
					gui_element->detected_inputs.is_hovered = true;
					gui_element->detected_inputs.is_left_mouse_button_on_press = input.is_left_mouse_button_on_press();
					gui_element->detected_inputs.is_left_mouse_button_on_release = input.is_left_mouse_button_on_release();
					gui_element->detected_inputs.is_left_mouse_button_double_clicked = input.is_left_mouse_button_double_clicked();
					gui_element->detected_inputs.is_middle_mouse_button_on_press = input.is_middle_mouse_button_on_press();
					gui_element->detected_inputs.is_middle_mouse_button_on_release = input.is_middle_mouse_button_on_release();
					gui_element->detected_inputs.is_middle_mouse_button_double_clicked = input.is_middle_mouse_button_double_clicked();
					gui_element->detected_inputs.is_right_mouse_button_on_press = input.is_right_mouse_button_on_press();
					gui_element->detected_inputs.is_right_mouse_button_on_release = input.is_right_mouse_button_on_release();
					gui_element->detected_inputs.is_right_mouse_button_double_clicked = input.is_right_mouse_button_double_clicked();
				}

				gui_element->update_image();
			}
			else
			{

			}

			gui->update_image();
		}
	}
}

void Omnific::GUISystem::finalize()
{
	this->is_initialized = false;
}