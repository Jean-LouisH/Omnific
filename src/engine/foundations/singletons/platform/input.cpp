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

#include "input.hpp"
#include <foundations/singletons/event_bus.hpp>
#include <foundations/singletons/platform/platform.hpp>

Omnific::Inputs::Inputs()
{
	this->gamepad_buttons_by_string.emplace("dpad_left", SDL_GAMEPAD_BUTTON_DPAD_LEFT);
	this->gamepad_buttons_by_string.emplace("dpad_right", SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
	this->gamepad_buttons_by_string.emplace("dpad_up", SDL_GAMEPAD_BUTTON_DPAD_UP);
	this->gamepad_buttons_by_string.emplace("dpad_down", SDL_GAMEPAD_BUTTON_DPAD_DOWN);
	this->gamepad_buttons_by_string.emplace("button_extra", SDL_GAMEPAD_BUTTON_BACK);
	this->gamepad_buttons_by_string.emplace("button_menu", SDL_GAMEPAD_BUTTON_START);
	this->gamepad_buttons_by_string.emplace("button_x", SDL_GAMEPAD_BUTTON_WEST);
	this->gamepad_buttons_by_string.emplace("button_y", SDL_GAMEPAD_BUTTON_NORTH);
	this->gamepad_buttons_by_string.emplace("button_a", SDL_GAMEPAD_BUTTON_SOUTH);
	this->gamepad_buttons_by_string.emplace("button_b", SDL_GAMEPAD_BUTTON_EAST);
	this->gamepad_buttons_by_string.emplace("left_shoulder_button", SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
	this->gamepad_buttons_by_string.emplace("left_stick_button", SDL_GAMEPAD_BUTTON_LEFT_STICK);
	this->gamepad_buttons_by_string.emplace("right_shoulder_button", SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
	this->gamepad_buttons_by_string.emplace("right_stick_button", SDL_GAMEPAD_BUTTON_RIGHT_STICK);

	this->gamepad_axis_events_by_string.emplace("left_axis_x", SDL_GAMEPAD_AXIS_LEFTX);
	this->gamepad_axis_events_by_string.emplace("left_axis_y", SDL_GAMEPAD_AXIS_LEFTY);
	this->gamepad_axis_events_by_string.emplace("right_axis_x", SDL_GAMEPAD_AXIS_RIGHTX);
	this->gamepad_axis_events_by_string.emplace("right_axis_y", SDL_GAMEPAD_AXIS_RIGHTY);
	this->gamepad_axis_events_by_string.emplace("left_trigger", SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
	this->gamepad_axis_events_by_string.emplace("right_trigger", SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);

	this->keyboard_events_by_string.emplace("escape", SDL_SCANCODE_ESCAPE);
	this->keyboard_events_by_string.emplace("f1", SDL_SCANCODE_F1);
	this->keyboard_events_by_string.emplace("f2", SDL_SCANCODE_F2);
	this->keyboard_events_by_string.emplace("f3", SDL_SCANCODE_F3);
	this->keyboard_events_by_string.emplace("f4", SDL_SCANCODE_F4);
	this->keyboard_events_by_string.emplace("f5", SDL_SCANCODE_F5);
	this->keyboard_events_by_string.emplace("f6", SDL_SCANCODE_F6);
	this->keyboard_events_by_string.emplace("f7", SDL_SCANCODE_F7);
	this->keyboard_events_by_string.emplace("f8", SDL_SCANCODE_F8);
	this->keyboard_events_by_string.emplace("f9", SDL_SCANCODE_F9);
	this->keyboard_events_by_string.emplace("f10", SDL_SCANCODE_F10);
	this->keyboard_events_by_string.emplace("f11", SDL_SCANCODE_F11);
	this->keyboard_events_by_string.emplace("f12", SDL_SCANCODE_F12);
	this->keyboard_events_by_string.emplace("printscreen", SDL_SCANCODE_PRINTSCREEN);
	this->keyboard_events_by_string.emplace("pause", SDL_SCANCODE_PAUSE);
	this->keyboard_events_by_string.emplace("insert", SDL_SCANCODE_INSERT);
	this->keyboard_events_by_string.emplace("delete", SDL_SCANCODE_DELETE);
	this->keyboard_events_by_string.emplace("home", SDL_SCANCODE_HOME);
	this->keyboard_events_by_string.emplace("page_up", SDL_SCANCODE_PAGEUP);
	this->keyboard_events_by_string.emplace("page_down", SDL_SCANCODE_PAGEDOWN);
	this->keyboard_events_by_string.emplace("end", SDL_SCANCODE_END);
	this->keyboard_events_by_string.emplace("grave", SDL_SCANCODE_GRAVE);
	this->keyboard_events_by_string.emplace("1", SDL_SCANCODE_1);
	this->keyboard_events_by_string.emplace("2", SDL_SCANCODE_2);
	this->keyboard_events_by_string.emplace("3", SDL_SCANCODE_3);
	this->keyboard_events_by_string.emplace("4", SDL_SCANCODE_4);
	this->keyboard_events_by_string.emplace("5", SDL_SCANCODE_5);
	this->keyboard_events_by_string.emplace("6", SDL_SCANCODE_6);
	this->keyboard_events_by_string.emplace("7", SDL_SCANCODE_7);
	this->keyboard_events_by_string.emplace("8", SDL_SCANCODE_8);
	this->keyboard_events_by_string.emplace("9", SDL_SCANCODE_9);
	this->keyboard_events_by_string.emplace("0", SDL_SCANCODE_0);
	this->keyboard_events_by_string.emplace("-", SDL_SCANCODE_MINUS);
	this->keyboard_events_by_string.emplace("=", SDL_SCANCODE_EQUALS);
	this->keyboard_events_by_string.emplace("backspace", SDL_SCANCODE_BACKSPACE);
	this->keyboard_events_by_string.emplace("num_lock", SDL_SCANCODE_NUMLOCKCLEAR);
	this->keyboard_events_by_string.emplace("forward_slash", SDL_SCANCODE_SLASH);
	this->keyboard_events_by_string.emplace("keypad_minus", SDL_SCANCODE_KP_MINUS);
	this->keyboard_events_by_string.emplace("keypad_plus", SDL_SCANCODE_KP_PLUS);
	this->keyboard_events_by_string.emplace("keypad_enter", SDL_SCANCODE_KP_ENTER);
	this->keyboard_events_by_string.emplace("keypad_1", SDL_SCANCODE_KP_1);
	this->keyboard_events_by_string.emplace("keypad_2", SDL_SCANCODE_KP_2);
	this->keyboard_events_by_string.emplace("keypad_3", SDL_SCANCODE_KP_3);
	this->keyboard_events_by_string.emplace("keypad_4", SDL_SCANCODE_KP_4);
	this->keyboard_events_by_string.emplace("keypad_5", SDL_SCANCODE_KP_5);
	this->keyboard_events_by_string.emplace("keypad_6", SDL_SCANCODE_KP_6);
	this->keyboard_events_by_string.emplace("keypad_7", SDL_SCANCODE_KP_7);
	this->keyboard_events_by_string.emplace("keypad_8", SDL_SCANCODE_KP_8);
	this->keyboard_events_by_string.emplace("keypad_9", SDL_SCANCODE_KP_9);
	this->keyboard_events_by_string.emplace("keypad_0", SDL_SCANCODE_KP_0);
	this->keyboard_events_by_string.emplace("keypad_period", SDL_SCANCODE_KP_PERIOD);
	this->keyboard_events_by_string.emplace("tab", SDL_SCANCODE_TAB);
	this->keyboard_events_by_string.emplace("q", SDL_SCANCODE_Q);
	this->keyboard_events_by_string.emplace("w", SDL_SCANCODE_W);
	this->keyboard_events_by_string.emplace("e", SDL_SCANCODE_E);
	this->keyboard_events_by_string.emplace("r", SDL_SCANCODE_R);
	this->keyboard_events_by_string.emplace("t", SDL_SCANCODE_T);
	this->keyboard_events_by_string.emplace("y", SDL_SCANCODE_T);
	this->keyboard_events_by_string.emplace("u", SDL_SCANCODE_T);
	this->keyboard_events_by_string.emplace("i", SDL_SCANCODE_I);
	this->keyboard_events_by_string.emplace("o", SDL_SCANCODE_O);
	this->keyboard_events_by_string.emplace("p", SDL_SCANCODE_P);
	this->keyboard_events_by_string.emplace("a", SDL_SCANCODE_A);
	this->keyboard_events_by_string.emplace("s", SDL_SCANCODE_S);
	this->keyboard_events_by_string.emplace("d", SDL_SCANCODE_D);
	this->keyboard_events_by_string.emplace("f", SDL_SCANCODE_F);
	this->keyboard_events_by_string.emplace("g", SDL_SCANCODE_G);
	this->keyboard_events_by_string.emplace("h", SDL_SCANCODE_H);
	this->keyboard_events_by_string.emplace("j", SDL_SCANCODE_J);
	this->keyboard_events_by_string.emplace("k", SDL_SCANCODE_K);
	this->keyboard_events_by_string.emplace("l", SDL_SCANCODE_L);
	this->keyboard_events_by_string.emplace("z", SDL_SCANCODE_Z);
	this->keyboard_events_by_string.emplace("x", SDL_SCANCODE_X);
	this->keyboard_events_by_string.emplace("c", SDL_SCANCODE_C);
	this->keyboard_events_by_string.emplace("v", SDL_SCANCODE_V);
	this->keyboard_events_by_string.emplace("b", SDL_SCANCODE_B);
	this->keyboard_events_by_string.emplace("n", SDL_SCANCODE_N);
	this->keyboard_events_by_string.emplace("m", SDL_SCANCODE_M);
	this->keyboard_events_by_string.emplace("left_square_bracket", SDL_SCANCODE_LEFTBRACKET);
	this->keyboard_events_by_string.emplace("right_square_bracket", SDL_SCANCODE_RIGHTBRACKET);
	this->keyboard_events_by_string.emplace("semicolon", SDL_SCANCODE_SEMICOLON);
	this->keyboard_events_by_string.emplace("apostrophe", SDL_SCANCODE_APOSTROPHE);
	this->keyboard_events_by_string.emplace("backslash", SDL_SCANCODE_BACKSLASH);
	this->keyboard_events_by_string.emplace("enter", SDL_SCANCODE_RETURN);
	this->keyboard_events_by_string.emplace("comma", SDL_SCANCODE_COMMA);
	this->keyboard_events_by_string.emplace("period", SDL_SCANCODE_PERIOD);
	this->keyboard_events_by_string.emplace("left_shift", SDL_SCANCODE_LSHIFT);
	this->keyboard_events_by_string.emplace("right_shift", SDL_SCANCODE_RSHIFT);
	this->keyboard_events_by_string.emplace("left_control", SDL_SCANCODE_LCTRL);
	this->keyboard_events_by_string.emplace("right_control", SDL_SCANCODE_RCTRL);
	//this->keyboard_events_by_string.emplace("function", SDL_SCANCODE_f);
	this->keyboard_events_by_string.emplace("os", SDL_SCANCODE_LGUI);
	this->keyboard_events_by_string.emplace("left_alt", SDL_SCANCODE_LALT);
	this->keyboard_events_by_string.emplace("space", SDL_SCANCODE_SPACE);
	this->keyboard_events_by_string.emplace("right_alt", SDL_SCANCODE_RALT);
	this->keyboard_events_by_string.emplace("left", SDL_SCANCODE_LEFT);
	this->keyboard_events_by_string.emplace("up", SDL_SCANCODE_UP);
	this->keyboard_events_by_string.emplace("right", SDL_SCANCODE_RIGHT);
	this->keyboard_events_by_string.emplace("down", SDL_SCANCODE_DOWN);

	this->action_button_map.insert({
		{"dpad_left", {"dpad_left", "f"}},
		{"dpad_right", {"dpad_right", "h"}},
		{"dpad_up", {"dpad_up", "t"}},
		{"dpad_down", {"dpad_down", "g"}},
		{"button_extra", {"button_extra", "e"}},
		{"button_menu", {"button_menu", "u"}},
		{"button_x", {"button_x", "j"}},
		{"button_y", {"button_y", "i"}},
		{"button_a", {"button_a", "k"}},
		{"button_b", {"button_b", "l"}},
		{"left_shoulder_button", {"left_shoulder_button", "q"}},
		{"left_stick_button", {"left_stick_button", "c"}},
		{"right_shoulder_button", {"right_shoulder_button", "o"}},
		{"right_stick_button", {"right_stick_button", "n"}}		
	});

	this->action_axis_map.insert({
		{"left_axis_x", {{"left_axis_x", "left_axis_x"}, {"a", "d"}}},
		{"left_axis_y", {{"left_axis_y", "left_axis_y"}, {"s", "w"}}},
		{"right_axis_x", {{"right_axis_x", "right_axis_x"}, {"semicolon", "backslash"}}},	
		{"right_axis_y", {{"right_axis_y", "right_axis_y"}, {"apostrophe", "left_square_bracket"}}}
	});
}

bool Omnific::Inputs::is_on_press(std::string input_code)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return this->is_on_press(input_codes);
}

bool Omnific::Inputs::is_on_press(std::vector<std::string> input_codes)
{
	return this->is_on_press(input_codes, 0);
}

bool Omnific::Inputs::is_on_press(std::vector<std::string> input_codes, PlayerID player_id)
{
	for (int i = 0; i < input_codes.size(); i++)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
			for (auto& event: EventBus::query_events_with_number_parameter(OMNIFIC_EVENT_KEY_ON_PRESS, "scancode", this->keyboard_events_by_string.at(input_code)))
				return true;

		if (this->gamepad_buttons_by_string.count(input_code))
		{
			SDL_GamepadButton controller_button_code = this->gamepad_buttons_by_string.at(input_code);
			if (this->gamepad_button_events.count(controller_button_code))
				for (auto& event: EventBus::query_events_with_number_parameter(OMNIFIC_EVENT_BUTTON_ON_PRESS, "scancode", controller_button_code))
					for (int i = 0; i < MAX_GAMEPAD_PLAYERS; ++i)
						if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == gamepad_button_events.at(controller_button_code).which)
							return true;
		}
	}

	return false;
}

bool Omnific::Inputs::is_on_double_press(std::string input_code, unsigned int time_interval)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return this->is_on_double_press(input_codes, time_interval);
}

bool Omnific::Inputs::is_on_double_press(std::vector<std::string> input_codes, unsigned int time_interval)
{
	return this->is_on_double_press(input_codes, time_interval, 0);
}

bool Omnific::Inputs::is_on_double_press(std::vector<std::string> input_codes, unsigned int timer_interval, PlayerID player_id)
{
	for (int i = 0; i < input_codes.size(); ++i)
	{

	}

	return false;
}

bool Omnific::Inputs::is_pressed(std::string input_code)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return this->is_pressed(input_codes);
}

bool Omnific::Inputs::is_pressed(std::vector<std::string> input_codes)
{
	return is_pressed(input_codes, 0);
}

bool Omnific::Inputs::is_pressed(std::vector<std::string> input_codes, PlayerID player_id)
{
	for (int i = 0; i < input_codes.size(); ++i)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
		{
			SDL_Scancode sdl_scan_code = this->keyboard_events_by_string.at(input_code);
			if (this->held_keys.count(sdl_scan_code))
				return true;
		}

		if (this->gamepad_buttons_by_string.count(input_code))
		{
			SDL_GamepadButton controller_button_code = this->gamepad_buttons_by_string.at(input_code);
			if (this->held_gamepad_buttons.count(controller_button_code))
					for (int i = 0; i < MAX_GAMEPAD_PLAYERS; ++i)
						if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == this->held_gamepad_buttons.at(controller_button_code))
							return true;
		}
	}

	return false;
}


bool Omnific::Inputs::is_pressed_interrupt(std::string input_code)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return this->is_on_press(input_codes);
}

bool Omnific::Inputs::is_pressed_interrupt(std::vector<std::string> input_codes)
{
	return this->is_on_press(input_codes, 0);
}

bool Omnific::Inputs::is_pressed_interrupt(std::vector<std::string> input_codes, PlayerID player_id)
{
	for (int i = 0; i < input_codes.size(); i++)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
		{
			SDL_Keycode sdl_key_code = this->keyboard_events_by_string.at(input_code);
			if (this->keyboard_events.count(sdl_key_code))
				if (this->keyboard_events.at(sdl_key_code).type == SDL_EVENT_KEY_DOWN)
					return true;
		}

		if (this->gamepad_buttons_by_string.count(input_code))
		{
			SDL_GamepadButton controller_button_code = this->gamepad_buttons_by_string.at(input_code);
			if (this->gamepad_button_events.count(controller_button_code))
				if (this->gamepad_button_events.at(controller_button_code).type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
					for (int i = 0; i < MAX_GAMEPAD_PLAYERS; ++i)
						if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == gamepad_button_events.at(controller_button_code).which)
							return true;
		}
	}

	return false;
}

bool Omnific::Inputs::is_on_release(std::string input_code)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return this->is_on_release(input_codes);
}

bool Omnific::Inputs::is_on_release(std::vector<std::string> input_codes)
{
	return this->is_on_release(input_codes, 0);
}

bool Omnific::Inputs::is_on_release(std::vector<std::string> input_codes, PlayerID player_id)
{
	for (int i = 0; i < input_codes.size(); i++)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
			for (auto& event: EventBus::query_events_with_number_parameter(OMNIFIC_EVENT_KEY_ON_RELEASE, "scancode", this->keyboard_events_by_string.at(input_code)))
				return true;

		if (this->gamepad_buttons_by_string.count(input_code))
		{
			SDL_GamepadButton controller_button_code = this->gamepad_buttons_by_string.at(input_code);
			if (this->gamepad_button_events.count(controller_button_code))
				for (auto& event: EventBus::query_events_with_number_parameter(OMNIFIC_EVENT_BUTTON_ON_RELEASE, "scancode", controller_button_code))
					for (int i = 0; i < MAX_GAMEPAD_PLAYERS; ++i)
						if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == gamepad_button_events.at(controller_button_code).which)
							return true;
		}
	}

	return false;
}

bool Omnific::Inputs::is_released(std::string input_code)
{
	return !this->is_pressed(input_code);
}

bool Omnific::Inputs::is_released(std::string input_code, PlayerID player_id)
{
	std::vector<std::string> input_codes;
	input_codes.push_back(input_code);
	return !this->is_pressed(input_codes, player_id);
}

float Omnific::Inputs::get_axis(std::string input_code)
{
	return this->get_axis(input_code, 0);
}

float Omnific::Inputs::get_axis(std::string input_code, PlayerID player_id)
{
	float axis = 0.0;

	if (this->gamepad_axis_events_by_string.count(input_code))
	{
		SDL_GamepadAxis controller_axis_code = this->gamepad_axis_events_by_string.at(input_code);
		if (gamepad_axis_events.count(controller_axis_code))
			for (int i = 0; i < MAX_GAMEPAD_PLAYERS; ++i)
				if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == gamepad_axis_events.at(controller_axis_code).which)
					axis = (double)(gamepad_axis_events.at(controller_axis_code).value) / pow(2.0, 15.0);

		if (input_code == "left_axis_y" || input_code == "right_axis_y")
			axis *= -1.0;
	}

	return axis;
}

bool Omnific::Inputs::is_left_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
}

bool Omnific::Inputs::is_left_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_LEFT);
}

bool Omnific::Inputs::is_left_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP;
}

bool Omnific::Inputs::is_left_mouse_button_released()
{
	return !this->is_left_mouse_button_pressed();
}

bool Omnific::Inputs::is_left_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
		this->mouse_button_event.clicks == 2;
}

bool Omnific::Inputs::is_middle_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
}

bool Omnific::Inputs::is_middle_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_MIDDLE);
}

bool Omnific::Inputs::is_middle_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP;
}

bool Omnific::Inputs::is_middle_mouse_button_released()
{
	return !this->is_middle_mouse_button_pressed();
}

bool Omnific::Inputs::is_middle_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
		this->mouse_button_event.clicks == 2;
}

bool Omnific::Inputs::is_right_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
}

bool Omnific::Inputs::is_right_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_RIGHT);
}

bool Omnific::Inputs::is_right_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP;
}

bool Omnific::Inputs::is_right_mouse_button_released()
{
	return !this->is_right_mouse_button_pressed();
}

bool Omnific::Inputs::is_right_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
		this->mouse_button_event.clicks == 2;
}

glm::vec2 Omnific::Inputs::get_mouse_position()
{
	return this->mouse_position;
}

glm::vec2 Omnific::Inputs::get_mouse_wheel_velocity()
{
	glm::vec2 vector2;
	vector2.x = this->mouse_wheel_event.x;
	vector2.y = this->mouse_wheel_event.y;
	return vector2;
}

glm::vec2 Omnific::Inputs::get_mouse_motion_velocity()
{
	glm::vec2 vector2;
	vector2.x = this->mouse_motion_event.xrel;
	vector2.y = this->mouse_motion_event.yrel;
	return vector2;
}

bool Omnific::Inputs::is_action_on_press(std::string action_input_code)
{
	bool result = false;
	if (this->action_button_map.count(action_input_code))
		result = this->is_on_press(this->action_button_map.at(action_input_code));
	return result;
}

bool Omnific::Inputs::is_action_on_double_press(std::string action_input_code, unsigned int time_interval)
{
	bool result = false;
	if (this->action_button_map.count(action_input_code))
		result = this->is_on_double_press(this->action_button_map.at(action_input_code), time_interval);
	return result;
}

bool Omnific::Inputs::is_action_pressed(std::string action_input_code)
{
	bool result = false;
	if (this->action_button_map.count(action_input_code))
		result = this->is_pressed(this->action_button_map.at(action_input_code));
	return result;
}

bool Omnific::Inputs::is_action_on_release(std::string action_input_code)
{
	bool result = false;
	if (this->action_button_map.count(action_input_code))
		result = this->is_on_release(this->action_button_map.at(action_input_code));
	return result;
}

bool Omnific::Inputs::is_action_released(std::string action_input_code)
{
	bool result = false;
	if (this->action_button_map.count(action_input_code))
		result = this->is_released(this->action_button_map.at(action_input_code)[0]);
	return result;
}

float Omnific::Inputs::get_action_axis(std::string action_input_code)
{
	float result = 0.0f;
	if (this->action_axis_map.count(action_input_code))
	{
		for (auto& axis : this->action_axis_map.at(action_input_code))
		{
			result += this->get_axis(axis.first);
			result += (float)(this->is_pressed(axis.second)) - (float)(this->is_pressed(axis.first));
		}
	}
	return result;
}

void Omnific::Inputs::set_relative_mouse_mode(bool value)
{
	SDL_SetWindowRelativeMouseMode(Platform::get_window().get_sdlwindow(), value);
}

bool Omnific::Inputs::is_drop_file_detected()
{
	return this->drop_event.type == SDL_EVENT_DROP_FILE;
}

std::string Omnific::Inputs::get_drop_file_path()
{
	return this->drop_event.data;
}

uint32_t Omnific::Inputs::get_drop_file_window_id()
{
	uint32_t drop_file_window_id = -1;

	if (this->is_drop_file_detected())
		drop_file_window_id = this->drop_event.windowID;

	return drop_file_window_id;
}

void Omnific::Inputs::clear()
{
	this->gamepad_button_events.clear();
	this->keyboard_events.clear();
	this->has_detected_input_changes = false;

	SDL_zero(this->mouse_wheel_event);
	SDL_zero(this->mouse_motion_event);
	SDL_zero(this->mouse_button_event);
	SDL_zero(this->window_event);
}

void Omnific::Inputs::poll_input_events()
{
	SDL_Event input_events;
	SDL_Gamepad* gamepad = NULL;
	SDL_JoystickID gamepad_id = 0;
	uint32_t gamepad_instance_id = 0;

	Logger& logger = Platform::get_logger();

	this->clear();

	while (SDL_PollEvent(&input_events))
	{
		switch (input_events.type)
		{
		case SDL_EVENT_QUIT:
			this->shutdown_request = true;
			break;

		case SDL_EVENT_KEY_DOWN:
			
			if (!EventBus::has_continuous_event(OMNIFIC_EVENT_KEY_PRESSED, std::to_string((int)input_events.key.scancode)))
			{
				EventBus::publish_event(OMNIFIC_EVENT_KEY_ON_PRESS, {}, {{"scancode", input_events.key.scancode}});
				EventBus::publish_event(OMNIFIC_EVENT_KEY_PRESSED, {}, {}, {}, {}, std::to_string((int)input_events.key.scancode), true);
			}
			this->held_keys.insert(input_events.key.scancode);
			this->keyboard_events.emplace(input_events.key.scancode, input_events.key);
			this->has_detected_input_changes = true;
			break;
		case SDL_EVENT_KEY_UP:
			if (EventBus::has_continuous_event(OMNIFIC_EVENT_KEY_PRESSED, std::to_string((int)input_events.key.scancode)))
			{
				EventBus::publish_event(OMNIFIC_EVENT_KEY_ON_RELEASE, {}, {{"scancode", input_events.key.scancode}});
				EventBus::remove_continuous_event(OMNIFIC_EVENT_KEY_PRESSED, std::to_string((int)input_events.key.scancode));
			}
			this->held_keys.erase(input_events.key.scancode);
			this->keyboard_events.emplace(input_events.key.scancode, input_events.key);
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			this->held_mouse_buttons.insert(input_events.button.button);
			this->mouse_button_event = input_events.button;
			this->has_detected_input_changes = true;
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			this->held_mouse_buttons.erase(input_events.button.button);
			this->mouse_button_event = input_events.button;
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_MOUSE_MOTION:
			this->mouse_motion_event = input_events.motion;
			this->mouse_position.x = this->mouse_motion_event.x;
			this->mouse_position.y = this->mouse_motion_event.y;
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			this->mouse_wheel_event = input_events.wheel;
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			this->held_gamepad_buttons.emplace((SDL_GamepadButton)input_events.gbutton.button, input_events.gbutton.which);
			this->gamepad_button_events.emplace((SDL_GamepadButton)input_events.gbutton.button, input_events.gbutton);
			this->has_detected_input_changes = true;
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			this->held_gamepad_buttons.erase((SDL_GamepadButton)input_events.gbutton.button);
			this->gamepad_button_events.emplace((SDL_GamepadButton)input_events.gbutton.button, input_events.gbutton);
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if (!this->gamepad_axis_events.count((SDL_GamepadAxis)input_events.gaxis.axis))
				this->gamepad_axis_events.emplace((SDL_GamepadAxis)input_events.gaxis.axis, input_events.gaxis);
			else
				this->gamepad_axis_events.at((SDL_GamepadAxis)input_events.gaxis.axis) = input_events.gaxis;
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_DROP_FILE:
			this->drop_event = input_events.drop;
			EventBus::publish_event(
			OMNIFIC_EVENT_FILE_DROPPED_ON_WINDOW, 
				{{"drop_file_path", this->drop_event.data}},
				{{"drop_file_window_id", (double)this->drop_event.windowID}}
			);
			this->has_detected_input_changes = true;
			break;

		case SDL_EVENT_GAMEPAD_ADDED:
			gamepad_instance_id = input_events.gdevice.which;
			for (int i = 0; i < MAX_GAMEPAD_PLAYERS; i++) 
			{
				if (!this->gamepad_players[i].is_active) 
				{
					this->gamepad_players[i].gamepad = SDL_OpenGamepad(gamepad_instance_id);
					if (this->gamepad_players[i].gamepad) 
					{
						this->gamepad_players[i].joystick_id = gamepad_instance_id;
						this->gamepad_players[i].is_active = true;
						logger.write_info("Assigned Gamepad to Player " + std::to_string(i + 1) + " (ID: " + std::to_string(gamepad_instance_id) + ")");
					}
					break;
				}
			}
			this->has_detected_input_changes = true;
			break;
			
		case SDL_EVENT_GAMEPAD_REMOVED:
			gamepad_instance_id = input_events.gdevice.which;
			for (int i = 0; i < MAX_GAMEPAD_PLAYERS; i++) 
			{
				if (gamepad_players[i].is_active && gamepad_players[i].joystick_id == gamepad_instance_id) 
				{
					SDL_CloseGamepad(gamepad_players[i].gamepad);
					gamepad_players[i].gamepad = NULL;
					gamepad_players[i].joystick_id = 0;
					gamepad_players[i].is_active = false;
					logger.write_info("Player " + std::to_string(i + 1) + " disconnected.");
					return;
				}
			}
			this->has_detected_input_changes = true;
			break;
		}
	}
}

bool Omnific::Inputs::has_requested_shutdown()
{
	bool result = this->shutdown_request;
	this->shutdown_request = false;
	return result;
}

bool Omnific::Inputs::has_requested_restart()
{
	bool result = this->restart_request;
	this->restart_request = false;
	return result;
}

void Omnific::Inputs::request_shutdown()
{
	this->shutdown_request = true;
}

void Omnific::Inputs::request_restart()
{
	this->restart_request = true;
}

bool Omnific::Inputs::get_has_detected_input_changes()
{
	return this->has_detected_input_changes;
}

bool Omnific::Inputs::has_requested_command_line()
{
	bool backquote_released = false;

	if (this->keyboard_events.count(SDL_SCANCODE_GRAVE))
		backquote_released = this->keyboard_events.at(SDL_SCANCODE_GRAVE).type == SDL_EVENT_KEY_UP;

	return backquote_released;	 
}
