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

Omnific::Inputs::Inputs()
{
	this->controller_buttons_by_string.emplace("dpad_left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	this->controller_buttons_by_string.emplace("dpad_right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	this->controller_buttons_by_string.emplace("dpad_up", SDL_CONTROLLER_BUTTON_DPAD_UP);
	this->controller_buttons_by_string.emplace("dpad_down", SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	this->controller_buttons_by_string.emplace("button_extra", SDL_CONTROLLER_BUTTON_BACK);
	this->controller_buttons_by_string.emplace("button_menu", SDL_CONTROLLER_BUTTON_START);
	this->controller_buttons_by_string.emplace("button_x", SDL_CONTROLLER_BUTTON_X);
	this->controller_buttons_by_string.emplace("button_y", SDL_CONTROLLER_BUTTON_Y);
	this->controller_buttons_by_string.emplace("button_a", SDL_CONTROLLER_BUTTON_A);
	this->controller_buttons_by_string.emplace("button_b", SDL_CONTROLLER_BUTTON_B);
	this->controller_buttons_by_string.emplace("left_shoulder_button", SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	this->controller_buttons_by_string.emplace("left_stick_button", SDL_CONTROLLER_BUTTON_LEFTSTICK);
	this->controller_buttons_by_string.emplace("right_shoulder_button", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	this->controller_buttons_by_string.emplace("right_stick_button", SDL_CONTROLLER_BUTTON_RIGHTSTICK);

	this->controller_axis_events_by_string.emplace("left_axis_x", SDL_CONTROLLER_AXIS_LEFTX);
	this->controller_axis_events_by_string.emplace("left_axis_y", SDL_CONTROLLER_AXIS_LEFTY);
	this->controller_axis_events_by_string.emplace("right_axis_x", SDL_CONTROLLER_AXIS_RIGHTX);
	this->controller_axis_events_by_string.emplace("right_axis_y", SDL_CONTROLLER_AXIS_RIGHTY);
	this->controller_axis_events_by_string.emplace("left_trigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	this->controller_axis_events_by_string.emplace("right_trigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	this->keyboard_events_by_string.emplace("escape", SDLK_ESCAPE);
	this->keyboard_events_by_string.emplace("f1", SDLK_F1);
	this->keyboard_events_by_string.emplace("f2", SDLK_F2);
	this->keyboard_events_by_string.emplace("f3", SDLK_F3);
	this->keyboard_events_by_string.emplace("f4", SDLK_F4);
	this->keyboard_events_by_string.emplace("f5", SDLK_F5);
	this->keyboard_events_by_string.emplace("f6", SDLK_F6);
	this->keyboard_events_by_string.emplace("f7", SDLK_F7);
	this->keyboard_events_by_string.emplace("f8", SDLK_F8);
	this->keyboard_events_by_string.emplace("f9", SDLK_F9);
	this->keyboard_events_by_string.emplace("f10", SDLK_F10);
	this->keyboard_events_by_string.emplace("f11", SDLK_F11);
	this->keyboard_events_by_string.emplace("f12", SDLK_F12);
	this->keyboard_events_by_string.emplace("printscreen", SDLK_PRINTSCREEN);
	this->keyboard_events_by_string.emplace("pause", SDLK_PAUSE);
	this->keyboard_events_by_string.emplace("insert", SDLK_INSERT);
	this->keyboard_events_by_string.emplace("delete", SDLK_DELETE);
	this->keyboard_events_by_string.emplace("home", SDLK_HOME);
	this->keyboard_events_by_string.emplace("page_up", SDLK_PAGEUP);
	this->keyboard_events_by_string.emplace("page_down", SDLK_PAGEDOWN);
	this->keyboard_events_by_string.emplace("end", SDLK_END);
	this->keyboard_events_by_string.emplace("backquote", SDLK_BACKQUOTE);
	this->keyboard_events_by_string.emplace("1", SDLK_1);
	this->keyboard_events_by_string.emplace("2", SDLK_2);
	this->keyboard_events_by_string.emplace("3", SDLK_3);
	this->keyboard_events_by_string.emplace("4", SDLK_4);
	this->keyboard_events_by_string.emplace("5", SDLK_5);
	this->keyboard_events_by_string.emplace("6", SDLK_6);
	this->keyboard_events_by_string.emplace("7", SDLK_7);
	this->keyboard_events_by_string.emplace("8", SDLK_8);
	this->keyboard_events_by_string.emplace("9", SDLK_9);
	this->keyboard_events_by_string.emplace("0", SDLK_0);
	this->keyboard_events_by_string.emplace("-", SDLK_MINUS);
	this->keyboard_events_by_string.emplace("+", SDLK_PLUS);
	this->keyboard_events_by_string.emplace("backspace", SDLK_BACKSPACE);
	this->keyboard_events_by_string.emplace("num_lock", SDLK_NUMLOCKCLEAR);
	this->keyboard_events_by_string.emplace("forward_slash", SDLK_SLASH);
	this->keyboard_events_by_string.emplace("asterisk", SDLK_ASTERISK);
	this->keyboard_events_by_string.emplace("keypad_minus", SDLK_KP_MINUS);
	this->keyboard_events_by_string.emplace("keypad_plus", SDLK_KP_PLUS);
	this->keyboard_events_by_string.emplace("keypad_enter", SDLK_KP_ENTER);
	this->keyboard_events_by_string.emplace("keypad_1", SDLK_KP_1);
	this->keyboard_events_by_string.emplace("keypad_2", SDLK_KP_2);
	this->keyboard_events_by_string.emplace("keypad_3", SDLK_KP_3);
	this->keyboard_events_by_string.emplace("keypad_4", SDLK_KP_4);
	this->keyboard_events_by_string.emplace("keypad_5", SDLK_KP_5);
	this->keyboard_events_by_string.emplace("keypad_6", SDLK_KP_6);
	this->keyboard_events_by_string.emplace("keypad_7", SDLK_KP_7);
	this->keyboard_events_by_string.emplace("keypad_8", SDLK_KP_8);
	this->keyboard_events_by_string.emplace("keypad_9", SDLK_KP_9);
	this->keyboard_events_by_string.emplace("keypad_0", SDLK_KP_0);
	this->keyboard_events_by_string.emplace("keypad_period", SDLK_KP_PERIOD);
	this->keyboard_events_by_string.emplace("tab", SDLK_TAB);
	this->keyboard_events_by_string.emplace("q", SDLK_q);
	this->keyboard_events_by_string.emplace("w", SDLK_w);
	this->keyboard_events_by_string.emplace("e", SDLK_e);
	this->keyboard_events_by_string.emplace("r", SDLK_r);
	this->keyboard_events_by_string.emplace("t", SDLK_t);
	this->keyboard_events_by_string.emplace("y", SDLK_y);
	this->keyboard_events_by_string.emplace("u", SDLK_u);
	this->keyboard_events_by_string.emplace("i", SDLK_i);
	this->keyboard_events_by_string.emplace("o", SDLK_o);
	this->keyboard_events_by_string.emplace("p", SDLK_p);
	this->keyboard_events_by_string.emplace("a", SDLK_a);
	this->keyboard_events_by_string.emplace("s", SDLK_s);
	this->keyboard_events_by_string.emplace("d", SDLK_d);
	this->keyboard_events_by_string.emplace("f", SDLK_f);
	this->keyboard_events_by_string.emplace("g", SDLK_g);
	this->keyboard_events_by_string.emplace("h", SDLK_h);
	this->keyboard_events_by_string.emplace("j", SDLK_j);
	this->keyboard_events_by_string.emplace("k", SDLK_k);
	this->keyboard_events_by_string.emplace("l", SDLK_l);
	this->keyboard_events_by_string.emplace("z", SDLK_z);
	this->keyboard_events_by_string.emplace("x", SDLK_x);
	this->keyboard_events_by_string.emplace("c", SDLK_c);
	this->keyboard_events_by_string.emplace("v", SDLK_v);
	this->keyboard_events_by_string.emplace("b", SDLK_b);
	this->keyboard_events_by_string.emplace("n", SDLK_n);
	this->keyboard_events_by_string.emplace("m", SDLK_m);
	this->keyboard_events_by_string.emplace("left_square_bracket", SDLK_LEFTBRACKET);
	this->keyboard_events_by_string.emplace("right_square_bracket", SDLK_RIGHTBRACKET);
	this->keyboard_events_by_string.emplace("semicolon", SDLK_SEMICOLON);
	this->keyboard_events_by_string.emplace("quote", SDLK_QUOTE);
	this->keyboard_events_by_string.emplace("backslash", SDLK_BACKSLASH);
	this->keyboard_events_by_string.emplace("enter", SDLK_RETURN);
	this->keyboard_events_by_string.emplace("comma", SDLK_COMMA);
	this->keyboard_events_by_string.emplace("period", SDLK_PERIOD);
	this->keyboard_events_by_string.emplace("left_shift", SDLK_LSHIFT);
	this->keyboard_events_by_string.emplace("right_shift", SDLK_RSHIFT);
	this->keyboard_events_by_string.emplace("left_control", SDLK_LCTRL);
	this->keyboard_events_by_string.emplace("right_control", SDLK_RCTRL);
	this->keyboard_events_by_string.emplace("function", SDLK_f);
	this->keyboard_events_by_string.emplace("os", SDLK_LGUI);
	this->keyboard_events_by_string.emplace("left_alt", SDLK_LALT);
	this->keyboard_events_by_string.emplace("space", SDLK_SPACE);
	this->keyboard_events_by_string.emplace("right_alt", SDLK_RALT);
	this->keyboard_events_by_string.emplace("left", SDLK_LEFT);
	this->keyboard_events_by_string.emplace("up", SDLK_UP);
	this->keyboard_events_by_string.emplace("right", SDLK_RIGHT);
	this->keyboard_events_by_string.emplace("down", SDLK_DOWN);

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
		{"right_axis_x", {{"right_axis_x", "right_axis_x"}, {";", "\""}}},	
		{"left_axis_y", {{"right_axis_y", "right_axis_y"}, {"'", "["}}}
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
	for (int i = 0; i < input_codes.size(); ++i)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
		{
			SDL_Keycode sdl_key_code = this->keyboard_events_by_string.at(input_code);
			if (this->keyboard_events.count(sdl_key_code))
				if (this->keyboard_events.at(sdl_key_code).type == SDL_KEYDOWN && !this->are_inputs_read)
					return true;
		}
		
		if (this->controller_buttons_by_string.count(input_code))
		{
			SDL_GameControllerButton controller_button_code = this->controller_buttons_by_string.at(input_code);
			if (this->controller_button_events.count(controller_button_code))
				if (this->get_controller_player_map().count(player_id))
					if (this->controller_button_events.at(controller_button_code).type == SDL_CONTROLLERBUTTONDOWN &&
						this->controller_button_events.at(controller_button_code).which == this->get_controller_player_map().at(player_id) && 
						!this->are_inputs_read)
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
			SDL_Keycode sdl_key_code = this->keyboard_events_by_string.at(input_code);
			if (this->held_keys.count(sdl_key_code))
				return true;
		}

		if (this->controller_buttons_by_string.count(input_code))
		{
			SDL_GameControllerButton controller_button_code = this->controller_buttons_by_string.at(input_code);
			if (this->held_controller_buttons.count(controller_button_code))
				if (this->get_controller_player_map().count(player_id))
					if (this->held_controller_buttons.at(controller_button_code) == this->get_controller_player_map().at(player_id))
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
	for (int i = 0; i < input_codes.size(); ++i)
	{
		std::string input_code = input_codes.at(i);

		if (this->keyboard_events_by_string.count(input_code))
		{
			SDL_Keycode sdl_key_code = this->keyboard_events_by_string.at(input_code);
			if (this->keyboard_events.count(sdl_key_code))
				if (this->keyboard_events.at(sdl_key_code).type == SDL_KEYUP && !this->are_inputs_read)
					return true;
		}
		
		if (this->controller_buttons_by_string.count(input_code))
		{
			SDL_GameControllerButton controller_button_code = this->controller_buttons_by_string.at(input_code);
			if (this->controller_button_events.count(controller_button_code))
				if (this->get_controller_player_map().count(player_id))
					if (this->controller_button_events.at(controller_button_code).type == SDL_CONTROLLERBUTTONUP &&
						this->controller_button_events.at(controller_button_code).which == this->get_controller_player_map().at(player_id) && 
						!this->are_inputs_read)
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

	if (this->controller_axis_events_by_string.count(input_code))
	{
		SDL_GameControllerAxis controller_axis_code = this->controller_axis_events_by_string.at(input_code);
		if (controller_axis_events.count(controller_axis_code))
			if (this->get_controller_player_map().count(player_id))
				if (controller_axis_events.at(controller_axis_code).which == this->get_controller_player_map().at(player_id))
					axis = (double)(controller_axis_events.at(controller_axis_code).value) / pow(2.0, 15.0);

		if (input_code == "left_axis_y" || input_code == "right_axis_y")
			axis *= -1.0;
	}

	return axis;
}

bool Omnific::Inputs::is_left_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONDOWN && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_left_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_LEFT);
}

bool Omnific::Inputs::is_left_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_left_mouse_button_released()
{
	return !this->is_left_mouse_button_pressed();
}

bool Omnific::Inputs::is_left_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_LEFT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP &&
		this->mouse_button_event.clicks == 2;
}

bool Omnific::Inputs::is_middle_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONDOWN && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_middle_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_MIDDLE);
}

bool Omnific::Inputs::is_middle_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_middle_mouse_button_released()
{
	return !this->is_middle_mouse_button_pressed();
}

bool Omnific::Inputs::is_middle_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_MIDDLE &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP &&
		this->mouse_button_event.clicks == 2;
}

bool Omnific::Inputs::is_right_mouse_button_on_press()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONDOWN && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_right_mouse_button_pressed()
{
	return this->held_mouse_buttons.count(SDL_BUTTON_RIGHT);
}

bool Omnific::Inputs::is_right_mouse_button_on_release()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP && 
		!this->are_inputs_read;
}

bool Omnific::Inputs::is_right_mouse_button_released()
{
	return !this->is_right_mouse_button_pressed();
}

bool Omnific::Inputs::is_right_mouse_button_double_clicked()
{
	return this->mouse_button_event.button == SDL_BUTTON_RIGHT &&
		this->mouse_button_event.type == SDL_MOUSEBUTTONUP &&
		this->mouse_button_event.clicks == 2;
}

glm::vec2 Omnific::Inputs::get_mouse_position()
{
	glm::vec2 vector2;
	vector2.x = this->mouse_motion_event.x;
	vector2.y = this->mouse_motion_event.y;
	return vector2;
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
		
	}
	return result;
}

bool Omnific::Inputs::is_drop_file_detected()
{
	return this->drop_event.type == SDL_DROPFILE;
}

std::string Omnific::Inputs::get_drop_file_path()
{
	return this->drop_event.file;
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
	//this->controller_axis_events.clear();
	this->controller_button_events.clear();
	this->keyboard_events.clear();

	if (drop_event.file != NULL)
	{
		SDL_free(drop_event.file);
		drop_event = { 0 };
	}

	this->has_detected_input_changes = false;
}

void Omnific::Inputs::detect_game_controllers()
{
	if (SDL_NumJoysticks() != this->game_controllers.size())
	{
		for (int i = 0; i < this->game_controllers.size(); ++i)
		{
			SDL_GameControllerClose(this->game_controllers.at(i));
			SDL_HapticClose(this->haptics.at(i));
		}

		this->game_controllers.clear();
		this->haptics.clear();
		this->controller_player_map.clear();

		for (int i = 0; i < SDL_NumJoysticks(); ++i)
		{
			if (SDL_IsGameController(i))
			{
				SDL_GameController* new_controller = SDL_GameControllerOpen(i);
				SDL_Joystick* joystick = SDL_GameControllerGetJoystick(new_controller);
				this->controller_player_map.emplace(i, SDL_JoystickInstanceID(joystick));
				this->game_controllers.push_back(new_controller);
				this->haptics.push_back(SDL_HapticOpenFromJoystick(joystick));
				this->newly_loaded_player_ids.push(i);
				SDL_HapticRumbleInit(this->haptics.back());
				if (!this->game_controllers.back())
					fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
}

void Omnific::Inputs::poll_input_events()
{
	SDL_Event SDLEvents;

	this->clear();

	while (SDL_PollEvent(&SDLEvents))
	{
		switch (SDLEvents.type)
		{
		case SDL_QUIT:
			this->shutdown_request = true;
			break;

		case SDL_KEYDOWN:
			this->held_keys.insert(SDLEvents.key.keysym.sym);
			this->keyboard_events.emplace(SDLEvents.key.keysym.sym, SDLEvents.key);
			this->has_detected_input_changes = true;
			break;
		case SDL_KEYUP:
			this->held_keys.erase(SDLEvents.key.keysym.sym);
			this->keyboard_events.emplace(SDLEvents.key.keysym.sym, SDLEvents.key);
			this->has_detected_input_changes = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
			this->held_mouse_buttons.insert(SDLEvents.button.button);
			this->mouse_button_event = SDLEvents.button;
			this->has_detected_input_changes = true;
			break;
		case SDL_MOUSEBUTTONUP:
			this->held_mouse_buttons.erase(SDLEvents.button.button);
			this->mouse_button_event = SDLEvents.button;
			this->has_detected_input_changes = true;
			break;

		case SDL_MOUSEMOTION:
			this->mouse_motion_event = SDLEvents.motion;
			this->has_detected_input_changes = true;
			break;

		case SDL_MOUSEWHEEL:
			this->mouse_wheel_event = SDLEvents.wheel;
			this->has_detected_input_changes = true;
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			this->held_controller_buttons.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton.which);
			this->controller_button_events.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton);
			this->has_detected_input_changes = true;
			break;
		case SDL_CONTROLLERBUTTONUP:
			this->held_controller_buttons.erase((SDL_GameControllerButton)SDLEvents.cbutton.button);
			this->controller_button_events.emplace((SDL_GameControllerButton)SDLEvents.cbutton.button, SDLEvents.cbutton);
			this->has_detected_input_changes = true;
			break;

		case SDL_CONTROLLERAXISMOTION:
			if (!this->controller_axis_events.count((SDL_GameControllerAxis)SDLEvents.caxis.axis))
				this->controller_axis_events.emplace((SDL_GameControllerAxis)SDLEvents.caxis.axis, SDLEvents.caxis);
			else
				this->controller_axis_events.at((SDL_GameControllerAxis)SDLEvents.caxis.axis) = SDLEvents.caxis;
			this->has_detected_input_changes = true;
			break;

		case SDL_DROPFILE:
			this->drop_event = SDLEvents.drop;
			this->has_detected_input_changes = true;
			break;
		}
	}
}

void Omnific::Inputs::on_start_of_frame()
{
	if (this->has_detected_input_changes)
		this->are_inputs_read = false;
}

void Omnific::Inputs::on_end_of_frame()
{
	this->are_inputs_read = true;
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

	if (this->keyboard_events.count(SDLK_BACKQUOTE))
		backquote_released = this->keyboard_events.at(SDLK_BACKQUOTE).type == SDL_KEYUP;

	return backquote_released;	 
}

std::vector<SDL_Haptic*> Omnific::Inputs::get_haptics()
{
	return this->haptics;
}

std::unordered_map<Omnific::PlayerID, SDL_JoystickID> Omnific::Inputs::get_controller_player_map()
{
	return this->controller_player_map;
}

std::queue<Omnific::PlayerID>& Omnific::Inputs::get_newly_loaded_player_ids()
{
	return this->newly_loaded_player_ids;
}