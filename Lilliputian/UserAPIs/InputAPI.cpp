#include "InputAPI.hpp"

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;

	/*Default action input map*/
	this->insertActionInput("dpad left", GAME_CONTROLLER_BUTTON_DPAD_LEFT);
	this->insertActionInput("dpad left", KEY_F);
	this->insertActionInput("dpad right", GAME_CONTROLLER_BUTTON_DPAD_RIGHT);
	this->insertActionInput("dpad right", KEY_H);
	this->insertActionInput("dpad up", GAME_CONTROLLER_BUTTON_DPAD_UP);
	this->insertActionInput("dpad up", KEY_T);
	this->insertActionInput("dpad down", GAME_CONTROLLER_BUTTON_DPAD_DOWN);
	this->insertActionInput("dpad down", KEY_G);
	this->insertActionInput("extra", GAME_CONTROLLER_BUTTON_EXTRA);
	this->insertActionInput("extra", KEY_E);
	this->insertActionInput("menu", GAME_CONTROLLER_BUTTON_MENU);
	this->insertActionInput("menu", KEY_U);
	this->insertActionInput("x", GAME_CONTROLLER_BUTTON_X);
	this->insertActionInput("x", KEY_J);
	this->insertActionInput("y", GAME_CONTROLLER_BUTTON_Y);
	this->insertActionInput("y", KEY_I);
	this->insertActionInput("a", GAME_CONTROLLER_BUTTON_A);
	this->insertActionInput("a", KEY_K);
	this->insertActionInput("b", GAME_CONTROLLER_BUTTON_B);
	this->insertActionInput("b", KEY_L);
	this->insertActionInput("left shoulder", GAME_CONTROLLER_BUTTON_LEFT_SHOULDER);
	this->insertActionInput("left shoulder", KEY_Q);
	this->insertActionInput("left stick", GAME_CONTROLLER_BUTTON_LEFT_STICK_BUTTON);
	this->insertActionInput("left stick", KEY_C);
	this->insertActionInput("right shoulder", GAME_CONTROLLER_BUTTON_RIGHT_SHOULDER);
	this->insertActionInput("right shoulder", KEY_O);
	this->insertActionInput("right stick", GAME_CONTROLLER_BUTTON_RIGHT_STICK_BUTTON);
	this->insertActionInput("right stick", KEY_N);
	this->insertActionInput("left axis left", GAME_CONTROLLER_LEFT_AXIS_LEFT);
	this->insertActionInput("left axis left", KEY_A);
	this->insertActionInput("left axis right", GAME_CONTROLLER_LEFT_AXIS_RIGHT);
	this->insertActionInput("left axis right", KEY_D);
	this->insertActionInput("left axis up", GAME_CONTROLLER_LEFT_AXIS_UP);
	this->insertActionInput("left axis up", KEY_W);
	this->insertActionInput("left axis down", GAME_CONTROLLER_LEFT_AXIS_DOWN);
	this->insertActionInput("left axis down", KEY_S);
	this->insertActionInput("right axis left", GAME_CONTROLLER_RIGHT_AXIS_LEFT);
	this->insertActionInput("right axis left", KEY_SEMICOLON);
	this->insertActionInput("right axis right", GAME_CONTROLLER_RIGHT_AXIS_RIGHT);
	this->insertActionInput("right axis right", KEY_BACKSLASH);
	this->insertActionInput("right axis up", GAME_CONTROLLER_RIGHT_AXIS_UP);
	this->insertActionInput("right axis up", KEY_QUOTE);
	this->insertActionInput("right axis down", GAME_CONTROLLER_RIGHT_AXIS_DOWN);
	this->insertActionInput("right axis down", KEY_OPEN_SQUARE_BRACKET);
	this->insertActionInput("left trigger", GAME_CONTROLLER_TRIGGER_LEFT_AXIS);
	this->insertActionInput("left trigger", KEY_LEFT_SHIFT);
	this->insertActionInput("right trigger", GAME_CONTROLLER_TRIGGER_RIGHT_AXIS);
	this->insertActionInput("right trigger", KEY_RIGHT_SHIFT);
}

void Lilliputian::InputAPI::insertActionInput(const char* actionName, uint8_t inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionName))
	{
		this->actionInputs.at(actionName).push_back(inputCode);
	}
	else
	{
		Vector<uint8_t> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}
}

void Lilliputian::InputAPI::replaceActionInput(const char* actionName, uint8_t inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionString))
	{
		this->actionInputs.at(actionString).clear();
		this->actionInputs.at(actionString).push_back(inputCode);
	}
	else
	{
		Vector<uint8_t> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}

}

void Lilliputian::InputAPI::removeActionInput(const char* actionName, uint8_t inputCode)
{

}

void Lilliputian::InputAPI::removeAllActionInputs(const char* actionName)
{
	String actionString = actionName;
	if (this->actionInputs.count(actionString))
		this->actionInputs.at(actionString).clear();
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Vector<uint8_t>> Lilliputian::InputAPI::getActionInputs()
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