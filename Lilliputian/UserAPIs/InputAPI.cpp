#include "InputAPI.hpp"

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;
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
		this->actionInputs.emplace(actionString, inputCodes);
	}
}

bool Lilliputian::InputAPI::isKeyPressed(unsigned int keyCode)
{
	return false;
}