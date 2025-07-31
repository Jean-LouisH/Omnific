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

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <foundations/aliases.hpp>
#include <glm.hpp>
#include <set>
#include <engine_api.hpp>
#include <string>
#include <atomic>
#include <array>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Inputs
	{
		friend class Engine;
	public:
		Inputs();

		bool is_on_press(std::string input_code);
		bool is_on_press(std::vector<std::string> input_codes);
		bool is_on_press(std::vector<std::string> input_codes, PlayerID player_id);
		bool is_on_double_press(std::string input_code, unsigned int time_interval);
		bool is_on_double_press(std::vector<std::string> input_codes, unsigned int timer_interval);
		bool is_on_double_press(std::vector<std::string> input_codes, unsigned int timer_interval, PlayerID player_id);
		bool is_pressed(std::string input_code);
		bool is_pressed(std::vector<std::string> input_codes);
		bool is_pressed(std::vector<std::string> input_codes, PlayerID player_id);
		bool is_on_release(std::string input_code);
		bool is_on_release(std::vector<std::string> input_codes);
		bool is_on_release(std::vector<std::string> input_codes, PlayerID player_id);
		bool is_released(std::string input_code);
		bool is_released(std::string input_code, PlayerID player_id);

		float get_axis(std::string input_code);
		float get_axis(std::string input_code, PlayerID player_id);

		bool is_left_mouse_button_on_press();
		bool is_left_mouse_button_pressed();
		bool is_left_mouse_button_on_release();
		bool is_left_mouse_button_released();
		bool is_left_mouse_button_double_clicked();
		bool is_middle_mouse_button_on_press();
		bool is_middle_mouse_button_pressed();
		bool is_middle_mouse_button_on_release();
		bool is_middle_mouse_button_released();
		bool is_middle_mouse_button_double_clicked();
		bool is_right_mouse_button_on_press();
		bool is_right_mouse_button_pressed();
		bool is_right_mouse_button_on_release();
		bool is_right_mouse_button_released();
		bool is_right_mouse_button_double_clicked();
		glm::vec2 get_mouse_position();
		glm::vec2 get_mouse_wheel_velocity();
		glm::vec2 get_mouse_motion_velocity();

		bool is_action_on_press(std::string action_input_code);
		bool is_action_on_double_press(std::string action_input_code, unsigned int time_interval);
		bool is_action_pressed(std::string action_input_code);
		bool is_action_on_release(std::string action_input_code);
		bool is_action_released(std::string action_input_code);
		float get_action_axis(std::string action_input_code);

		bool is_drop_file_detected();
		std::string get_drop_file_path();
		uint32_t get_drop_file_window_id();

		void request_shutdown();
		void request_restart();

		void reset_edge_transition_detections();

		bool has_requested_shutdown();
		bool has_requested_restart();
		bool has_requested_command_line();
		bool get_has_detected_input_changes();

		std::vector<SDL_Haptic*> get_haptics();
		std::unordered_map<PlayerID, SDL_JoystickID> get_controller_player_map();
		std::queue<PlayerID>& get_newly_loaded_player_ids();
	private:
		void detect_game_controllers();
		void poll_input_events();

		std::unordered_map<std::string, SDL_Scancode> keyboard_events_by_string;
		std::unordered_map<std::string, SDL_GameControllerButton> controller_buttons_by_string;
		std::unordered_map<std::string, SDL_GameControllerAxis> controller_axis_events_by_string;

		std::unordered_map<std::string, std::vector<std::string>> action_button_map;
		std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> action_axis_map;

		std::set<std::string> held_inputs;

		std::set<SDL_Keycode> held_keys;
		std::set<Uint8> held_mouse_buttons;

		std::array<std::atomic<bool>, SDL_NUM_SCANCODES> on_press_keys;
		std::array<std::atomic<bool>, SDL_NUM_SCANCODES> on_release_keys;
		std::array<std::atomic<bool>, SDL_CONTROLLER_BUTTON_MAX> on_press_controller_buttons;
		std::array<std::atomic<bool>, SDL_CONTROLLER_BUTTON_MAX> on_release_controller_buttons;
		std::array<std::atomic<bool>, 6> on_press_mouse_buttons;
		std::array<std::atomic<bool>, 6> on_release_mouse_buttons;

		std::array<int, SDL_NUM_SCANCODES> on_press_key_indices;
		std::array<int, SDL_NUM_SCANCODES> on_release_key_indices;
		std::array<int, SDL_CONTROLLER_BUTTON_MAX> on_press_controller_button_indices;
		std::array<int, SDL_CONTROLLER_BUTTON_MAX> on_release_controller_button_indices;
		std::array<int, 6> on_press_mouse_button_indices;
		std::array<int, 6> on_release_mouse_button_indices;

		int on_press_key_count = 0;
		int on_release_key_count = 0;
		int on_press_controller_button_count = 0;
		int on_release_controller_button_count = 0;
		int on_press_mouse_button_count = 0;
		int on_release_mouse_button_count = 0;
		
		std::unordered_map<SDL_GameControllerButton, SDL_JoystickID> held_controller_buttons;
		std::unordered_map<SDL_GameControllerButton, SDL_ControllerButtonEvent> controller_button_events;
		std::unordered_map<SDL_Keycode, SDL_KeyboardEvent> keyboard_events;
		std::unordered_map<SDL_GameControllerAxis, SDL_ControllerAxisEvent> controller_axis_events;
		SDL_MouseButtonEvent mouse_button_event = { 0 };
		SDL_MouseMotionEvent mouse_motion_event = { 0 };
		SDL_MouseWheelEvent  mouse_wheel_event = { 0 };
		SDL_WindowEvent window_event = { 0 };
		SDL_DropEvent drop_event = { 0 };

		std::vector<SDL_GameController*> game_controllers;
		std::vector<SDL_Haptic*> haptics;

		std::unordered_map<PlayerID, SDL_JoystickID> controller_player_map;
		std::queue<PlayerID> newly_loaded_player_ids;

		std::atomic<bool> has_detected_input_changes = false;
		bool shutdown_request = false;
		bool restart_request = false;
		void clear();
	};
}