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
// THE SOFTWARE IS PROVIDED "AS IS", InputCode::WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "input_api.hpp"

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;

	/*Default action input map*/
	this->insertActionInput("dpad left", InputCode::GAME_CONTROLLER_BUTTON_DPAD_LEFT);
	this->insertActionInput("dpad left", InputCode::KEY_F);
	this->insertActionInput("dpad right", InputCode::GAME_CONTROLLER_BUTTON_DPAD_RIGHT);
	this->insertActionInput("dpad right", InputCode::KEY_H);
	this->insertActionInput("dpad up", InputCode::GAME_CONTROLLER_BUTTON_DPAD_UP);
	this->insertActionInput("dpad up", InputCode::KEY_T);
	this->insertActionInput("dpad down", InputCode::GAME_CONTROLLER_BUTTON_DPAD_DOWN);
	this->insertActionInput("dpad down", InputCode::KEY_G);
	this->insertActionInput("extra", InputCode::GAME_CONTROLLER_BUTTON_EXTRA);
	this->insertActionInput("extra", InputCode::KEY_E);
	this->insertActionInput("menu", InputCode::GAME_CONTROLLER_BUTTON_MENU);
	this->insertActionInput("menu", InputCode::KEY_U);
	this->insertActionInput("x", InputCode::GAME_CONTROLLER_BUTTON_X);
	this->insertActionInput("x", InputCode::KEY_J);
	this->insertActionInput("y", InputCode::GAME_CONTROLLER_BUTTON_Y);
	this->insertActionInput("y", InputCode::KEY_I);
	this->insertActionInput("a", InputCode::GAME_CONTROLLER_BUTTON_A);
	this->insertActionInput("a", InputCode::KEY_K);
	this->insertActionInput("b", InputCode::GAME_CONTROLLER_BUTTON_B);
	this->insertActionInput("b", InputCode::KEY_L);
	this->insertActionInput("left shoulder", InputCode::GAME_CONTROLLER_BUTTON_LEFT_SHOULDER);
	this->insertActionInput("left shoulder", InputCode::KEY_Q);
	this->insertActionInput("left stick", InputCode::GAME_CONTROLLER_BUTTON_LEFT_STICK_BUTTON);
	this->insertActionInput("left stick", InputCode::KEY_C);
	this->insertActionInput("right shoulder", InputCode::GAME_CONTROLLER_BUTTON_RIGHT_SHOULDER);
	this->insertActionInput("right shoulder", InputCode::KEY_O);
	this->insertActionInput("right stick", InputCode::GAME_CONTROLLER_BUTTON_RIGHT_STICK_BUTTON);
	this->insertActionInput("right stick", InputCode::KEY_N);
	this->insertActionInput("left axis left", InputCode::GAME_CONTROLLER_LEFT_AXIS_LEFT);
	this->insertActionInput("left axis left", InputCode::KEY_A);
	this->insertActionInput("left axis right", InputCode::GAME_CONTROLLER_LEFT_AXIS_RIGHT);
	this->insertActionInput("left axis right", InputCode::KEY_D);
	this->insertActionInput("left axis up", InputCode::GAME_CONTROLLER_LEFT_AXIS_UP);
	this->insertActionInput("left axis up", InputCode::KEY_W);
	this->insertActionInput("left axis down", InputCode::GAME_CONTROLLER_LEFT_AXIS_DOWN);
	this->insertActionInput("left axis down", InputCode::KEY_S);
	this->insertActionInput("right axis left", InputCode::GAME_CONTROLLER_RIGHT_AXIS_LEFT);
	this->insertActionInput("right axis left", InputCode::KEY_SEMICOLON);
	this->insertActionInput("right axis right", InputCode::GAME_CONTROLLER_RIGHT_AXIS_RIGHT);
	this->insertActionInput("right axis right", InputCode::KEY_BACKSLASH);
	this->insertActionInput("right axis up", InputCode::GAME_CONTROLLER_RIGHT_AXIS_UP);
	this->insertActionInput("right axis up", InputCode::KEY_QUOTE);
	this->insertActionInput("right axis down", InputCode::GAME_CONTROLLER_RIGHT_AXIS_DOWN);
	this->insertActionInput("right axis down", InputCode::KEY_OPEN_SQUARE_BRACKET);
	this->insertActionInput("left trigger", InputCode::GAME_CONTROLLER_TRIGGER_LEFT_AXIS);
	this->insertActionInput("left trigger", InputCode::KEY_LEFT_SHIFT);
	this->insertActionInput("right trigger", InputCode::GAME_CONTROLLER_TRIGGER_RIGHT_AXIS);
	this->insertActionInput("right trigger", InputCode::KEY_RIGHT_SHIFT);
}

void Lilliputian::InputAPI::insertActionInput(const char* actionName, InputCode inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionName))
	{
		this->actionInputs.at(actionName).push_back(inputCode);
	}
	else
	{
		Vector<InputCode> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}
}

void Lilliputian::InputAPI::replaceActionInput(const char* actionName, InputCode inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionString))
	{
		this->actionInputs.at(actionString).clear();
		this->actionInputs.at(actionString).push_back(inputCode);
	}
	else
	{
		Vector<InputCode> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}

}

void Lilliputian::InputAPI::removeActionInput(const char* actionName, InputCode inputCode)
{

}

void Lilliputian::InputAPI::removeAllActionInputs(const char* actionName)
{
	String actionString = actionName;
	if (this->actionInputs.count(actionString))
		this->actionInputs.at(actionString).clear();
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Vector<Lilliputian::InputCode>> Lilliputian::InputAPI::getActionInputs()
{
	return this->actionInputs;
}

bool Lilliputian::InputAPI::isKeyOnPress(unsigned int keyCode)
{
	return false;
}

bool Lilliputian::InputAPI::isKeyOnDoublePress(unsigned int keyCode, unsigned int timeInterval_ms)
{
	return false;
}

bool Lilliputian::InputAPI::isKeyOnRelease(unsigned int keyCode)
{
	return false;
}

bool Lilliputian::InputAPI::isKeyOnHold(unsigned int keyCode)
{
	return false;
}

bool Lilliputian::InputAPI::isActionOnPress(const char* actionName)
{
	return false;
}

bool Lilliputian::InputAPI::isActionOnDoublePress(const char* actionName, unsigned int timeInterval_ms)
{
	return false;
}

bool Lilliputian::InputAPI::isActionOnRelease(const char* actionName)
{
	return false;
}

bool Lilliputian::InputAPI::isActionOnHold(const char* actionName, unsigned int timeInterval_ms)
{
	return false;
}

float Lilliputian::InputAPI::getAxisStrength(const char* axisActionName)
{
	return 0.0;
}